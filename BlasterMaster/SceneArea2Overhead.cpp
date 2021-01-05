#include <iostream>
#include <fstream>

#include "Sound.h"
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
#include "ItemPower.h"
#include "Teleporter.h"
#include "Cannon.h"
#include "ItemGun.h"
#include "Eyeball.h"
#include "EyeballSpawner.h"
#include "Breakable_Obstacle.h"

#include "QuadTree.h"
#include "SceneOpening.h"
#include "SceneBoss.h"

using namespace std;

static D3DCOLOR enterColor[8] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(210,255,255,255),D3DCOLOR_ARGB(180,255,255,255),D3DCOLOR_ARGB(135,255,255,255),D3DCOLOR_ARGB(90,255,255,255), D3DCOLOR_ARGB(50,255,255,255), D3DCOLOR_ARGB(0,255,255,255),D3DCOLOR_ARGB(10,255,255,255) };

BoundingBox SceneArea2Overhead::cameraLimitAreaOfSection[9] = {
	// section 1
	BoundingBox(0, 0, 528, 280),
	// section 2
	BoundingBox(0, 256, 526, 782),
	//section 3
	BoundingBox(1280, 768, 1550, 1294),
	// section 4
	BoundingBox(0, 1792, 270, 2072),
	// section 1 for 2
	BoundingBox(512,0,1040,528),
	// section 2 for 2
	BoundingBox(512,496,782,776),
	// section 5 boss
	BoundingBox(768, 768, 1038, 1048),
	// section win it
	BoundingBox(768, 512, 1038, 792)
};

Point SceneArea2Overhead::startPointInSection[6] = {
	// section A
	Point(75, 116),
	// section B
	Point(74, 368),
	//section C
	Point(1402, 1136),
	// section D
	Point(120, 1908),
	// section E boss
	Point(896,804),
	// win boss
	Point(896,692)
};

SceneArea2Overhead::SceneArea2Overhead(int id, LPCWSTR filePath, Game *game, Point screenSize) : Scene(id, filePath, game)
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
	this->bulletState = 0;
	this->enterBoss = 0;
	this->countEnterBoss = 0;
	this->count = 0;
}

void SceneArea2Overhead::LoadContent()
{
	mMap = new GameMap("Map/General/level2-over-tiless.tmx", textureLib, spriteLib);
	foreMap = new GameMap("Map/General/level2-over-fores.tmx", textureLib, spriteLib);

	healthBar = new HealthBar(textureLib, spriteLib);
	gunBar = new GunBar(textureLib, spriteLib);
	bulletscene = new SceneBullet(textureLib, spriteLib);

	// camera setup
	mCamera = new Camera(Point(GameGlobal::GetWidth(), GameGlobal::GetHeight()));
	Camera::setCameraInstance(mCamera);
	/*mCamera->SetPosition(mMap->GetWidth() / 2 + GameGlobal::GetWidth() / 2,
		mMap->GetHeight() / 2 + GameGlobal::GetHeight() / 2 + 16);*/

	/*mCamera->SetPosition(GameGlobal::GetWidth() / 2,
		mMap->GetHeight() - GameGlobal::GetHeight() / 2 + 32);*/

	// set limit area of section 1
	mCamera->SetCameraLimitarea(GameGlobal::GetReturnBoundingBox());
	mMap->SetCamera(mCamera);
	foreMap->SetCamera(mCamera);

}

SceneArea2Overhead::~SceneArea2Overhead()
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
	bulletscene->Release();
	delete bulletscene;
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

#define OBJECT_TYPE_WORM 1
#define OBJECT_TYPE_JUMPER 2
#define OBJECT_TYPE_TELEPORTER 3
#define OBJECT_TYPE_CANNON 4
#define OBJECT_TYPE_DOME 5
#define OBJECT_TYPE_EYE 6
#define OBJECT_TYPE_MINE 7
#define OBJECT_TYPE_FLOATER 8
#define OBJECT_TYPE_INSECT 9
#define OBJECT_TYPE_ORB 10
#define OBJECT_TYPE_WALKER 11
#define OBJECT_TYPE_JASON_OVERHEAD 12
#define OBJECT_TYPE_EYESPAWNER 13
#define OBJECT_TYPE_HOMING_BULLET 204
#define OBJECT_TYPE_MULTI_BULLET 205
#define OBJECT_TYPE_CRUSHER_BEAM 207
#define OBJECT_TYPE_GUN 203
#define OBJECT_TYPE_THUNDER 206
#define OBJECT_TYPE_POWER 201

#define OBJECT_TYPE_Breakable_Obstacle 298

//LeSon
#define ENVIRONMENT_TYPE_WALL 1
#define ENVIRONMENT_TYPE_SPIKE 2
#define ENVIRONMENT_TYPE_PORTAL 3
#define ENVIRONMENT_TYPE_LADDER 4
#define ENVIRONMENT_TYPE_LAVA 5
#define ENVIRONMENT_TYPE_OUTDOOR 6
#define ENVIRONMENT_TYPE_ENTERBOSS 10
#define ENVIRONMENT_TYPE_UNKNOWN -1

#define MAX_SCENE_LINE 1024

void SceneArea2Overhead::_ParseSection_TEXTURES(string line)
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

void SceneArea2Overhead::_ParseSection_SPRITES(string spritePath)
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

void SceneArea2Overhead::_ParseSection_ANIMATIONS(string animationPath)
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

			Animation *ani = new Animation();

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

void SceneArea2Overhead::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2)
		return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	AnimationSet *s = new AnimationSet();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		Animation *ani = animationLib->Get(ani_id);
		s->push_back(ani);
	}

	animationSetLib->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void SceneArea2Overhead::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3)
		return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());


	GameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_JASON_OVERHEAD:
		obj = new JasonOverhead(x, y);
		obj->SetPosition(GameGlobal::GetReturnPoint());
		break;
	case OBJECT_TYPE_Breakable_Obstacle:
		obj = new Breakable_Obstacle(x, y, 0);
		break;
	case OBJECT_TYPE_TELEPORTER:
		obj = new Teleporter(x, y);
		break;
	case OBJECT_TYPE_CANNON:
		obj = new Cannon(x, y);
		break;
	case OBJECT_TYPE_EYE:
		obj = new Eyeball(x, y);
		break;
	case OBJECT_TYPE_EYESPAWNER:
		obj = new EyeballSpawner(x, y);
        break;
	case OBJECT_TYPE_POWER:
		obj = new ItemPower(Point(x, y), 1);
		break;
	case OBJECT_TYPE_HOMING_BULLET:
		obj = new ItemGun(Point(x, y), 1);
		break;
	case OBJECT_TYPE_MULTI_BULLET:
		obj = new ItemGun(Point(x, y), 3);
		break;
	case OBJECT_TYPE_THUNDER:
		obj = new ItemGun(Point(x, y), 2);
		break;
	case OBJECT_TYPE_GUN:
		obj = new ItemGun(Point(x, y), 4);
		break;
	case OBJECT_TYPE_CRUSHER_BEAM:
		obj = new ItemGun(Point(x, y), 5);
		break;
	/*case OBJECT_TYPE_WALKER:
		obj = new Walker(x, y);
		break;*/
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
		AnimationSet *ani_set = animationSetLib->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}
	objects.insert(obj);
}

//LeSon
void SceneArea2Overhead::_ParseSection_ENVIRONMENT(string line)
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

	Environment *env = NULL;

	switch (env_type)
	{
	case ENVIRONMENT_TYPE_WALL:
		env = new Env_Wall(x, y, width, height);
		break;
	case ENVIRONMENT_TYPE_SPIKE:
		env = new Env_Spike(x, y, width, height);
		break;
	case ENVIRONMENT_TYPE_LAVA:
		env = new Env_Lava(x, y, width, height);
		break;
	case ENVIRONMENT_TYPE_PORTAL:
		if (dirId == 0) {
			gateDir = LEFT;
		}
		else if (dirId == 1) {
			gateDir = RIGHT;
		}
		else if (dirId == 2) {
			gateDir = TOP;
		}
		else {
			gateDir = BOTTOM;
		}
		env = new Env_Portal(x, y, width, height, gateDir, sectionToEnter, sectionLocation);
		break;
	case ENVIRONMENT_TYPE_OUTDOOR:
		if (dirId == 0) {
			gateDir = LEFT;
		}
		else if (dirId == 1) {
			gateDir = RIGHT;
		}
		else if (dirId == 2) {
			gateDir = TOP;
		}
		else {
			gateDir = BOTTOM;
		}
		env = new Env_Outdoor(x, y, width, height, gateDir, sectionToEnter);
		break;
	case ENVIRONMENT_TYPE_ENTERBOSS:
		env = new Env_Enterboss(x, y, width, height);
		break;
	default:
		DebugOut(L"[ERR] Invalid env type: %d\n", env_type);
		return;
	}

	environments.push_back(env);
	DebugOut(L"[INFO] An environment add type=%d, x=%f, y=%f, width=%f, height=%f\n", env_type, x, y, width, height);
}

void SceneArea2Overhead::Init()
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

	/*target = NULL;
	for (auto x : objects) {
		Player* current_player = dynamic_cast<Player*>(x);
		if (current_player != NULL &&
			current_player->IsPrimaryPlayer()) {
			mCamera->SetTarget(current_player);
			target = current_player;
		}
	}*/

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

void SceneArea2Overhead::JumpCheckpoint()
{
	if (!isCameraFree) {
		Input& input = *GameGlobal::GetInput();
		// section A
		if (input[0x30]) {
			target->SetPosition(startPointInSection[0]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[0]);
		}
		// section B
		else if (input[0x31]) {
			target->SetPosition(startPointInSection[1]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[1]);
		}
		//section C
		else if (input[0x32]) {
			target->SetPosition(startPointInSection[2]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[2]);
		}
		else if (input[0x33]) {
			target->SetPosition(startPointInSection[3]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[3]);
		}
		else if (input[0x34]) {
			target->SetPosition(startPointInSection[4]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[6]);
		}
	}
}

#define FRAME_PORTAL_TRANSITIONS 130
#define DISTANCE_JASON_PORTAL_LEFT_RIGHT 35
#define DISTANCE_JASON_PORTAL_UP_DOWN 50
#define DURATION_ENTER 600

void SceneArea2Overhead::Update()
{
	input->Update();
	if (!bulletState) {
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

		// update onscreen objects
		vector<GameObject*> onScreenObj;
		for (auto x : objects) {
			if (x->GetBoundingBox().IsOverlap(mCamera->GetBound())) {
				onScreenObj.push_back(x);
			}
		}

		Camera::setCameraInstance(mCamera);
		if (!isCameraFree && enterBoss == 0) {
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
				this->Release();
				Game::GetInstance()->Init(L"Resources/scene.txt", 3);
				return;
			}
			GameGlobal::SetCurrentHealthPoint(target->GetHP());
			mCamera->FollowTarget();
			mCamera->SnapToBoundary();

			//LeSon
			/*/
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
				quadTree.InsertAndInteract(x, x->GetBoundingBox().Drag(x->dx()));
			}
			//*/

			// Long
			/*
			for (int i = 0; i < onScreenObj.size(); i++)
				for (int j = i + 1; j < onScreenObj.size(); j++)
					onScreenObj[i]->Interact(onScreenObj[j]);
			//*/

			for (auto object : onScreenObj)
			{
				object->Update();
			}


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

			if (directionEnterPortal == 1) {
				mCamera->SetPosition(mCamera->GetPosition() + Point(2, 0));
				target->SetPosition(target->GetPosition() + Point(0.4, 0));
			}
			else if (directionEnterPortal == 0) {
				mCamera->SetPosition(mCamera->GetPosition() + Point(-2, 0));
				target->SetPosition(target->GetPosition() - Point(0.4, 0));
			}
			else if (directionEnterPortal == 2) {
				mCamera->SetPosition(mCamera->GetPosition() + Point(0, -2));
				target->SetPosition(target->GetPosition() - Point(0, 0.4));
			}
			else if (directionEnterPortal == 3) {
				mCamera->SetPosition(mCamera->GetPosition() + Point(0, 2));
				target->SetPosition(target->GetPosition() + Point(0, 0.4));
			}
			frameToTransition++;
			//DebugOut(L"Frame to transition: %d", frameToTransition);
			if (frameToTransition >= FRAME_PORTAL_TRANSITIONS) {
				if (directionEnterPortal == 1) {
					target->SetPosition(target->GetPosition() + Point(DISTANCE_JASON_PORTAL_LEFT_RIGHT, 0));
				}
				else if (directionEnterPortal == 0) {
					target->SetPosition(target->GetPosition() - Point(DISTANCE_JASON_PORTAL_LEFT_RIGHT, 0));
				}
				else if (directionEnterPortal == 2) {
					target->SetPosition(target->GetPosition() - Point(0, DISTANCE_JASON_PORTAL_UP_DOWN));
				}
				else if (directionEnterPortal == 3) {
					target->SetPosition(target->GetPosition() + Point(0, DISTANCE_JASON_PORTAL_UP_DOWN));
				}
				isCameraFree = false;
				directionEnterPortal = -1;
				frameToTransition = 0;
			}
		}

		if (enterBoss == 1) {
			countEnterBoss++;
		}

		if (countEnterBoss > DURATION_ENTER) {
			this->Release();
			Sound::getInstance()->stop("entering_boss_scene");
			Sound::getInstance()->play("area2", false, 1);
			Game::GetInstance()->Init(L"Resources/scene.txt", 5);
			SceneBoss* scene = dynamic_cast<SceneBoss*>(Game::GetInstance()->GetCurrentScene());
			if (scene != NULL) {
				scene->liveShow = 0;
				for (auto x : scene->GetObjects()) {
					JasonOverhead* jason = dynamic_cast<JasonOverhead*>(x);
					if (jason != NULL) {
						jason->SetHP(GameGlobal::GetCurrentHealthPoint());
						break;
					}
				}
			}
			return;
		}

		// enter to switch scene
		//if ((*input)[VK_BACK] & KEY_STATE_DOWN) {
		//	//Game::GetInstance()->SwitchScene(3);
		//	this->Release();
		//	Game::GetInstance()->Init(L"Resources/scene.txt", 2);
		//	return;
		//}

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
}

void SceneArea2Overhead::displayBulletState() {
	Input& input = *GameGlobal::GetInput();
	if (input[VK_RETURN] & KEY_STATE_DOWN) {
		bulletState = true;
		Sound::getInstance()->play("scene_change", false, 1);
	}
}

void SceneArea2Overhead::backToGame() {
	Input& input = *GameGlobal::GetInput();
	if (input[VK_BACK] & KEY_STATE_DOWN) {
		bulletState = false;
		Sound::getInstance()->play("scene_change", false, 1);
	}
}

void SceneArea2Overhead::Render()
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
		//Sound::getInstance()->stop("enter");
		//Sound::getInstance()->play("area2", true, 0);
		if (countEnterBoss == 0) {
			if (!bulletState) {
				displayBulletState();
			}
			else {
				backToGame();
			}
		}
		count = DURATION_OF_LIVESHOW + 1;
		mMap->Draw();
		for (auto object : objects) {
			if (dynamic_cast<Breakable_Obstacle*>(object) != NULL) {
				object->Render();
			}
		}

		for (auto object : objects) {
			if (dynamic_cast<Breakable_Obstacle*>(object) == NULL && dynamic_cast<SceneBox1*>(object) == NULL) {
				ItemGun* item = dynamic_cast<ItemGun*>(object);
				if(item == NULL || (item != NULL && item->type != 5) || (GameGlobal::GetWinBoss() == 1 && item != NULL && item->type == 5))
					object->Render();
			}
		}

		foreMap->Draw();
		if (enterBoss == 1 && GameGlobal::GetWinBoss() == 0) {
			for (auto object : objects) {
				if (dynamic_cast<SceneBox1*>(object) != NULL) {
					object->drawArguments.SetColor(enterColor[rand() % 8]);
					object->Render();
				}
			}
		}
		else {
			healthBar->Draw();
			gunBar->Draw();
			if (bulletState) {
				bulletscene->Draw();
			}
		}
	}
}

/*
	Unload current scene
*/
void SceneArea2Overhead::Release()
{
	for (auto object : objects)
		delete object;

	objects.clear();

	// map release sucks hihi
	mMap->Release();
	foreMap->Release();

	Scene::Release();

	healthBar->Release();
	bulletscene->Release();
	gunBar->Release();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

Player * SceneArea2Overhead::GetTarget()
{
	if (this == nullptr) return NULL;
	return target;
}

Camera * SceneArea2Overhead::GetCamera()
{
	return mCamera;
}

void SceneArea2Overhead::SetTarget(Player * player)
{
	this->target = player;
}

unordered_set<GameObject*> SceneArea2Overhead::GetObjects()
{
	return objects;
}

void SceneArea2Overhead::AddElement(GameObject* obj)
{
	objects.insert(obj);
}

void SceneArea2Overhead::RemoveElement(GameObject * obj)
{
	for (int i = 0; i < toRemove.size(); i++)
		if (toRemove[i] == obj)
			return;
	toRemove.push_back(obj);
}

