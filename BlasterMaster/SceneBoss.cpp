#include "SceneBoss.h"
#include <iostream>
#include <fstream>

#include "SceneArea2Overhead.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"

#include "Worm.h"
#include "JasonOverhead.h"
#include "GameObject.h"
#include "AnimatedGameObject.h"
#include "Jumper.h"
#include "Dome.h"
#include "Floater.h"
#include "Teleporter.h"
#include "Cannon.h"
#include "Eyeball.h"
#include "Breakable_Obstacle.h"
#include "Boss.h"
#include "SceneOpening.h"


#include "QuadTree.h"

using namespace std;

BoundingBox SceneBoss::cameraLimitAreaOfSection[9] = {
	// section 1
	BoundingBox(0, 0, 270, 280)
};

Point SceneBoss::startPointInSection[5] = {
	// section A
	Point(75, 116)
};

SceneBoss::SceneBoss(int id, LPCWSTR filePath, Game* game, Point screenSize) : Scene(id, filePath, game)
{
	this->input = game->GetInput();
	LoadContent();
	//mMap = new GameMap("Map/General/level2-side-tiless.tmx", textureLib, spriteLib);
	this->screenSize = screenSize;
	this->isCameraFree = false;
	this->directionEnterPortal = -1;
	this->frameToTransition = 0;
	LoadLivesLeftDisplay(textureLib, spriteLib);
	this->liveShow = 1;
	this->count = 0;
}

void SceneBoss::LoadContent()
{
	mMap = new GameMap("Map/General/level2-over-tiless.tmx", textureLib, spriteLib);
	foreMap = new GameMap("Map/General/level2-over-fores.tmx", textureLib, spriteLib);

	healthBar = new HealthBar(textureLib, spriteLib);
	gunBar = new GunBar(textureLib, spriteLib);

	// camera setup
	mCamera = new Camera(Point(GameGlobal::GetWidth(), GameGlobal::GetHeight()));
	Camera::setCameraInstance(mCamera);

	// set limit area of section 1
	mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[0]);
	mMap->SetCamera(mCamera);
	foreMap->SetCamera(mCamera);

}

SceneBoss::~SceneBoss()
{
	for (auto i : objects)
		delete i;
	objects.clear();
	mMap->Release();
	delete mMap;
	foreMap->Release();
	delete foreMap;
	healthBar->Release();
	delete healthBar;
	gunBar->Release();
	delete gunBar;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS 5
#define SCENE_SECTION_OBJECTS 6
#define SCENE_SECTION_MAP 7
//LeSon
#define SCENE_SECTION_ENVIRONMENT 8

//#define OBJECT_TYPE_PORTAL 50

#define OBJECT_TYPE_BBOX 505

#define OBJECT_TYPE_JASON_OVERHEAD 12
#define OBJECT_TYPE_BOSS 21

//LeSon
#define ENVIRONMENT_TYPE_WALL 1


#define MAX_SCENE_LINE 1024

void SceneBoss::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5)
		return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	textureLib->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void SceneBoss::_ParseSection_SPRITES(string spritePath)
{
	ifstream f;
	f.open(spritePath);

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "")
			continue;
		else
		{
			vector<string> tokens = split(line);

			if (tokens.size() < 6)
				return; // skip invalid lines

			int ID = atoi(tokens[0].c_str());

			BoundingBox bbox;
			bbox.l = atoi(tokens[1].c_str());
			bbox.t = atoi(tokens[2].c_str());
			bbox.r = atoi(tokens[3].c_str());
			bbox.b = atoi(tokens[4].c_str());
			int texID = atoi(tokens[5].c_str());

			LPDIRECT3DTEXTURE9 tex = textureLib->Get(texID);
			if (tex == NULL)
			{
				DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
				return;
			}

			spriteLib->Add(ID, bbox, tex);
		}
	}
	f.close();
}

void SceneBoss::_ParseSection_ANIMATIONS(string animationPath)
{
	ifstream f;
	f.open(animationPath);

	bool addedAnimations = false; // check if complete adding all animations
	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#' || line == "")
			continue;

		if (line == "[ANIMATION_SETS]")
		{
			addedAnimations = true;
			continue;
		}

		if (!addedAnimations)
		{
			vector<string> tokens = split(line);

			if (tokens.size() < 3)
				return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

			//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

			Animation* ani = new Animation();

			int ani_id = atoi(tokens[0].c_str());
			for (int i = 1; i < tokens.size(); i += 2) // why i+=2 ?  sprite_id | frame_time
			{
				int sprite_id = atoi(tokens[i].c_str());
				int frame_time = atoi(tokens[i + 1].c_str());
				ani->Add(sprite_id, spriteLib, frame_time);
			}

			animationLib->Add(ani_id, ani);
		}
		else
		{
			_ParseSection_ANIMATION_SETS(line);
		}
	}

	f.close();
}

void SceneBoss::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2)
		return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	AnimationSet* s = new AnimationSet();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		Animation* ani = animationLib->Get(ani_id);
		s->push_back(ani);
	}

	animationSetLib->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void SceneBoss::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3)
		return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());


	GameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON_OVERHEAD:
		obj = new JasonOverhead(x, y);
		break;
	case OBJECT_TYPE_BOSS:
		obj = new Boss(x, y);
		break;
	case OBJECT_TYPE_BBOX:
		obj = new SceneBox1(x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	obj->SetManager(this);

	//hihi
	for (int i = 3; i < tokens.size(); i++)
	{
		int ani_set_id = atoi(tokens[i].c_str());
		AnimationSet* ani_set = animationSetLib->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}
	objects.insert(obj);
}

//LeSon
void SceneBoss::_ParseSection_ENVIRONMENT(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 5)
		return; // skip invalid lines - environment must have id, x y, width, height

	int env_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	float width = atof(tokens[3].c_str());
	float height = atof(tokens[4].c_str());

	int dirId = -1;
	int sectionToEnter = -1;
	int sectionLocation = -1;
	if (tokens.size() >= 7) {
		dirId = atoi(tokens[5].c_str());
		sectionToEnter = atoi(tokens[6].c_str());
		if (tokens.size() == 8) {
			sectionLocation = atoi(tokens[7].c_str());
		}
	}

	GateDirection gateDir;

	Environment* env = NULL;

	switch (env_type)
	{
	case ENVIRONMENT_TYPE_WALL:
		env = new Env_Wall(x, y, width, height);
		break;
	default:
		DebugOut(L"[ERR] Invalid env type: %d\n", env_type);
		return;
	}

	environments.push_back(env);
	DebugOut(L"[INFO] An environment add type=%d, x=%f, y=%f, width=%f, height=%f\n", env_type, x, y, width, height);
}

void SceneBoss::Init()
{
	//vector<tuple<int, int, int, int, int>> mapNav;
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	// move bbox init
	textureLib->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#')
			continue; // skip comment lines

		if (line == "[TEXTURES]")
		{
			section = SCENE_SECTION_TEXTURES;
			continue;
		}
		if (line == "[SPRITES]")
		{
			section = SCENE_SECTION_SPRITES;
			continue;
		}
		if (line == "[ANIMATIONS]")
		{
			section = SCENE_SECTION_ANIMATIONS;
			continue;
		}
		if (line == "[ANIMATION_SETS]")
		{
			section = SCENE_SECTION_ANIMATION_SETS;
			continue;
		}
		if (line == "[OBJECTS]")
		{
			section = SCENE_SECTION_OBJECTS;
			continue;
		}
		// LeSon
		if (line == "[ENVIRONMENTS]")
		{
			section = SCENE_SECTION_ENVIRONMENT;
			continue;
		}
		/*
		if (line == "[MAP]")
		{
			section = SCENE_SECTION_MAP;
			continue;
		}*/
		if (line[0] == '[')
		{
			section = SCENE_SECTION_UNKNOWN;
			continue;
		}

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES:
			_ParseSection_TEXTURES(line);
			break;
		case SCENE_SECTION_SPRITES:
			_ParseSection_SPRITES(line);
			break;
		case SCENE_SECTION_ANIMATIONS:
			_ParseSection_ANIMATIONS(line);
			break;
		case SCENE_SECTION_ANIMATION_SETS:
			_ParseSection_ANIMATION_SETS(line);
			break;
		case SCENE_SECTION_OBJECTS:
			_ParseSection_OBJECTS(line);
			break;
			//LeSon
		case SCENE_SECTION_ENVIRONMENT:
			_ParseSection_ENVIRONMENT(line);
			break;
			/*
			case SCENE_SECTION_MAP:
				_ParseSection_MAP(line, mapNav);
				break;*/
		}
	}

	target = NULL;
	for (auto x : objects) {
		Player* current_player = dynamic_cast<Player*>(x);
		if (current_player != NULL &&
			current_player->IsPrimaryPlayer()) {
			mCamera->SetTarget(current_player);
			target = current_player;
		}
	}

	f.close();

	//// NAK son
	//// NAK tien
	//for (auto item : mapNav)
	//{
	//	int id = item._Myfirst._Val;
	//	int top = item._Get_rest()._Myfirst._Val;
	//	int left = item._Get_rest()._Get_rest()._Myfirst._Val;
	//	int bottom = item._Get_rest()._Get_rest()._Get_rest()._Myfirst._Val;
	//	int right = item._Get_rest()._Get_rest()._Get_rest()._Get_rest()._Myfirst._Val;

	//	map[id]->PartialInit(map[top], map[left], map[bottom], map[right]);
	//}

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	//displayMessage("yeah");
}

void SceneBoss::JumpCheckpoint()
{
	if (!isCameraFree) {
		Input& input = *GameGlobal::GetInput();
		// section A
		if (input[0x30]) {
			target->SetPosition(startPointInSection[0]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[0]);
		}
	}
}

#define FRAME_PORTAL_TRANSITIONS 130
#define DISTANCE_JASON_PORTAL_LEFT_RIGHT 35
#define DISTANCE_JASON_PORTAL_UP_DOWN 50

void SceneBoss::Update()
{
	// Quick & dirty
	if (count < DURATION_OF_LIVESHOW) {
		return;
	}

	GameGlobal::SetAnimationSetLibrary(animationSetLib);

	// Remove all things that need to remove last frame
	for (auto obj : toRemove)
	{
		objects.erase(obj);
		delete obj;
	}
	toRemove.clear();

	input->Update();
	// update onscreen objects
	vector<GameObject*> onScreenObj;
	for (auto x : objects) {
		if (x->GetBoundingBox().IsOverlap(mCamera->GetBound())) {
			onScreenObj.push_back(x);
		}
	}

	Camera::setCameraInstance(mCamera);
	if (!isCameraFree) {
		target = NULL;
		for (auto x : objects) {
			Player* current_player = dynamic_cast<Player*>(x);
			if (current_player != NULL &&
				current_player->IsPrimaryPlayer()) {
				mCamera->SetTarget(current_player);
				target = current_player;
			}
		}
		if (target == NULL)
		{
			int currentLivesPlay = GameGlobal::GetLivesToPlay();
			GameGlobal::SetLivesToPlay(currentLivesPlay - 1);
			count = 0;
			if (currentLivesPlay == 0) {// change later for continue game
				GameGlobal::SetLivesToPlay(2);
				GameGlobal::SetReturnPoint(Point(56, 2955));
				GameGlobal::SetReturnBoundingBox(BoundingBox(0, 2814, 1038, 3094));
				this->Release();
				Game::GetInstance()->Init(L"Resources/scene.txt", 4);
				return;
			}
			//TODO: set again start pos when return play
			GameGlobal::SetReturnPoint(SceneArea2Overhead::startPointInSection[4]);
			GameGlobal::SetReturnBoundingBox(SceneArea2Overhead::cameraLimitAreaOfSection[6]);
			//this->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 3);
			return;
		}
		GameGlobal::SetCurrentHealthPoint(target->GetHP());
		mCamera->FollowTarget();
		mCamera->SnapToBoundary();

		//LeSon
		//*/
		for (auto x : onScreenObj) {
			for (auto y : environments) {
				x->Interact((Interactable*)y);
			}
		}
		/*/
		// Quad Tree, by Long
		QuadTree quadTree;
		for (auto y : environments) {
			quadTree.InsertToTree(y, y->GetBoundingBox());
		}
		for (auto x : onScreenObj) {
			if (dynamic_cast<Breakable_Obstacle*>(x) != NULL) {
				quadTree.InsertAndInteract(x, dynamic_cast<Breakable_Obstacle*>(x)->GetBoundingBoxJason());
			}
			else {
				// If there are any non-proximity-based interaction, detect & handle here
				quadTree.InsertAndInteract(x, x->GetBoundingBox());
			}
		}
		//*/

		for (auto object : onScreenObj)
		{
			object->Update();
			// boss next
			Boss* boss = dynamic_cast<Boss*>(object);
			if (boss != NULL && boss->HealthPoint <= 0) {
				/*GameGlobal::SetWinBoss(true);
				GameGlobal::SetReturnPoint(SceneArea2Overhead::startPointInSection[4]);
				GameGlobal::SetReturnBoundingBox(SceneArea2Overhead::cameraLimitAreaOfSection[6]);
				this->Release();
				Game::GetInstance()->Init(L"Resources/scene.txt", 3);
				return;*/
			}
		}

		// Long
		//*
		for (int i = 0; i < onScreenObj.size(); i++)
			for (int j = i + 1; j < onScreenObj.size(); j++)
				onScreenObj[i]->Interact(onScreenObj[j]);
		//*/

		// temporary global set hp for both sophia jason
	}
	else {
		// update enemies when change section
		for (auto x : onScreenObj) {
			bool isPlayer = dynamic_cast<Player*>(x) != NULL;
			if (!isPlayer) {
				for (auto y : environments) {
					x->Interact((Interactable*)y);
				}
				x->Update();
			}
		}
	}

	JumpCheckpoint();

	//if ((*input)[VK_LEFT] & KEY_STATE_DOWN)
	//{
	//	if (mCamera->GetPosition().x - mCamera->GetWidth() / 2 <= 0) {
	//		return;
	//	} // LeSon
	//	// sau nay doi lai la thay doi vi tri nhan vat, camera se setPosition theo vi tri nhan vat
	//	mCamera->SetPosition(mCamera->GetPosition() + Point(-8, 0));
	//}
	//if ((*input)[VK_RIGHT] & KEY_STATE_DOWN)
	//{
	//	if (mCamera->GetPosition().x + mCamera->GetWidth() / 2 >= mMap->GetWidth() + 8) {
	//		return;
	//	}// LeSon
	//	// sau nay doi lai la thay doi vi tri nhan vat, camera se setPosition theo vi tri nhan vat
	//	mCamera->SetPosition(mCamera->GetPosition() + Point(8, 0));
	//}
	//if ((*input)[VK_UP] & KEY_STATE_DOWN)
	//{
	//	if (mCamera->GetPosition().y - mCamera->GetHeight() / 2 <= 0) return; // LeSon
	//	// sau nay doi lai la thay doi vi tri nhan vat, camera se setPosition theo vi tri nhan vat
	//	mCamera->SetPosition(mCamera->GetPosition() + Point(0, -8));
	//}
	//if ((*input)[VK_DOWN] & KEY_STATE_DOWN)
	//{
	//	if (mCamera->GetPosition().y + mCamera->GetHeight() / 2 >= mMap->GetHeight() + 32) return; // LeSon
	//	// sau nay doi lai la thay doi vi tri nhan vat, camera se setPosition theo vi tri nhan vat
	//	mCamera->SetPosition(mCamera->GetPosition() + Point(0, 8));
	//}

	// Update camera to follow mario
	Point pos;
	/*
	pos.x -= game->GetScreenWidth() / 2;
	pos.y -= game->GetScreenHeight() / 2;
	//*/
	game->SetCamPos(pos);
}

void SceneBoss::Render()
{
	// LeSon
	int currentLivesPlay = GameGlobal::GetLivesToPlay();
	if (count < DURATION_OF_LIVESHOW && currentLivesPlay >= 0 && liveShow == true)
	{
		DebugOut(L"count: %d\n", count);
		displayLivesLeft(currentLivesPlay);
		count++;
	}
	else
	{
		count = DURATION_OF_LIVESHOW + 1;
		//mMap->Draw();
		for (auto object : objects) {
			if (dynamic_cast<SceneBox1*>(object) != NULL) {
				object->Render();
			}
		}
		for (auto object : objects) {
			if (dynamic_cast<JasonOverhead*>(object) == NULL && dynamic_cast<SceneBox1*>(object) == NULL) {
				object->Render();
			}
		}
		for (auto object : objects) {
			if (dynamic_cast<JasonOverhead*>(object) != NULL) {
				object->Render();
			}
		}
		//foreMap->Draw();
		healthBar->Draw();
		gunBar->Draw();
	}
}

/*
	Unload current scene
*/
void SceneBoss::Release()
{
	for (auto object : objects)
		delete object;

	objects.clear();

	// map release sucks hihi
	mMap->Release();
	foreMap->Release();

	Scene::Release();

	healthBar->Release();

	gunBar->Release();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

Player* SceneBoss::GetTarget()
{
	if (this == nullptr) return NULL;
	return target;
}

Camera* SceneBoss::GetCamera()
{
	return mCamera;
}

void SceneBoss::SetTarget(Player* player)
{
	this->target = player;
}

unordered_set<GameObject*> SceneBoss::GetObjects()
{
	return objects;
}

void SceneBoss::AddElement(GameObject* obj)
{
	objects.insert(obj);
}

void SceneBoss::RemoveElement(GameObject* obj)
{
	for (int i = 0; i < toRemove.size(); i++)
		if (toRemove[i] == obj)
			return;
	toRemove.push_back(obj);
}

