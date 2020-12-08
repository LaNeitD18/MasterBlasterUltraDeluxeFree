#include <iostream>
#include <fstream>

#include "SceneArea2SideView.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"

#include "Worm.h"
#include "Sophia.h"
#include "GameObject.h"
#include "AnimatedGameObject.h"
#include "Jumper.h"
#include "Dome.h"
#include "Floater.h"
#include "Insect.h"
#include "Ship.h"
#include "Cannon.h"
#include "Orb.h"
#include "Skull.h"
#include "SkullBullet.h"
#include "Mine.h"
#include "Teleporter.h"

using namespace std;

// temporary set limit area for section, but not handle switch SceneOverhead, thinking of dividing into several SceneOverhead for easy win hihi
BoundingBox SceneArea2SideView::cameraLimitAreaOfSection[19] = {
	// section A
	BoundingBox(0, 2814, 1038, 3094),
	// section B
	BoundingBox(1024, 1792, 1550, 3094),
	//section C
	BoundingBox(1536, 1792, 2062, 2072),
	// section D
	BoundingBox(2048, 1024, 2574, 2072),
	// section E
	BoundingBox(2560, 1792, 3086, 2072),
	//section F
	BoundingBox(1536, 0, 2062, 1814),
	// section G
	BoundingBox(2048, 512, 2574, 792),
	// section H
	BoundingBox(2048, 0, 2574, 280),
	// section I
	BoundingBox(2560, 0, 3086, 568),
	// section J
	BoundingBox(2048, 256, 2574, 536),
	// section K
	BoundingBox(1280, 1024, 1550, 1304),
	// dungeon 1
	BoundingBox(2560, 1792, 3086, 2072),
	// dungeon 2
	BoundingBox(2048, 512, 2574, 792),
	// dungeon 3
	BoundingBox(1536, 32, 2062, 1814),
	//dungeon 4
	BoundingBox(2048, 256, 2574, 536),
	// section L
	BoundingBox(768,1534,1550,1814),
	// section M
	BoundingBox(512,1024,782,1814),
	//section N
	BoundingBox(768,1024,1294,1548),
	//section O
	BoundingBox(1280,1270,1550,1550)
};

Point SceneArea2SideView::startPointInSection[19] = {
	// section A
	Point(56, 2955),
	// section B
	Point(1076, 2955),
	//section C
	Point(1584, 1932),
	// section D
	Point(2096, 1932),
	// section E
	Point(2608, 1932),
	//section F
	Point(2000, 1164),
	// section G
	Point(2096, 652),
	// section H
	Point(2096, 140),
	// section I
	Point(2608, 140),
	// section J
	Point(2512,396),
	// section K
	Point(1488, 1164),
	// dungeon 1
	Point(3008,1932),
	// dungeon 2
	Point(2432,716),
	// dungeon 3
	Point(1600,140),
	// dungeon 4
	Point(2112,428),
	// section L
	Point(1488, 1674),
	// section M
	Point(720, 1674),
	// section N
	Point(816,1158),
	//section O
	Point(1328,1418)
};

SceneArea2SideView::SceneArea2SideView(int id, LPCWSTR filePath, Game *game, Point screenSize) : Scene(id, filePath, game)
{
	this->input = game->GetInput();
	LoadContent();
	//mMap = new GameMap("Map/General/level2-side-tiless.tmx", textureLib, spriteLib);
	this->screenSize = screenSize;
	this->isCameraFree = false;
	this->directionEnterPortal = -1;
	this->frameToTransition = 0;
	LoadLivesLeftDisplay(textureLib, spriteLib);
	this->count = 0;
}

void SceneArea2SideView::LoadContent()
{
	mMap = new GameMap("Map/General/level2-side-maporder.tmx", textureLib, spriteLib);
	foreMap = new GameMap("Map/General/level2-side-fores.tmx", textureLib, spriteLib);

	healthBar = new HealthBar(textureLib, spriteLib);

	// camera setup
	mCamera = new Camera(Point(GameGlobal::GetWidth(), GameGlobal::GetHeight()));
	Camera::setCameraInstance(mCamera);
	/*mCamera->SetPosition(mMap->GetWidth() / 2 + GameGlobal::GetWidth() / 2,
		mMap->GetHeight() / 2 + GameGlobal::GetHeight() / 2 + 16);*/

	/*mCamera->SetPosition(GameGlobal::GetWidth() / 2,
		mMap->GetHeight() - GameGlobal::GetHeight() / 2 + 32);*/
	
	// set limit area of section 1
	mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[0]);
	mMap->SetCamera(mCamera);
	foreMap->SetCamera(mCamera);
	//mMap->Draw();
	
}

SceneArea2SideView::~SceneArea2SideView()
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
#define OBJECT_TYPE_SOPHIA 12
#define OBJECT_TYPE_JASON_SIDE_VIEW 13
#define OBJECT_TYPE_SHIP 18
#define OBJECT_TYPE_SKULL 20
#define OBJECT_TYPE_SKULL_BULLET 100

//LeSon
#define ENVIRONMENT_TYPE_WALL 1
#define ENVIRONMENT_TYPE_SPIKE 2
#define ENVIRONMENT_TYPE_PORTAL 3
#define ENVIRONMENT_TYPE_LADDER 4
#define ENVIRONMENT_TYPE_LAVA 5
#define ENVIRONMENT_TYPE_DUNGEON 6
#define ENVIRONMENT_TYPE_UNKNOWN -1

#define MAX_SCENE_LINE 1024

void SceneArea2SideView::_ParseSection_TEXTURES(string line)
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

void SceneArea2SideView::_ParseSection_SPRITES(string spritePath)
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

void SceneArea2SideView::_ParseSection_ANIMATIONS(string animationPath)
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
				continue; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

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
			//displayMessage(animationLib->Get(ani_id)->GetLoopDuration());
		}
		else
		{
			_ParseSection_ANIMATION_SETS(line);
		}
	}

	f.close();
}

void SceneArea2SideView::_ParseSection_ANIMATION_SETS(string line)
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
void SceneArea2SideView::_ParseSection_OBJECTS(string line)
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
		/*case OBJECT_TYPE_MARIO:
			if (player!=NULL) 
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = new CMario(x,y); 
			player = (CMario*)obj;  

			DebugOut(L"[INFO] Player object created!\n");
			break;
		case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
		case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
		case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
		case OBJECT_TYPE_PORTAL:
			{	
				float r = atof(tokens[4].c_str());
				float b = atof(tokens[5].c_str());
				int scene_id = atoi(tokens[6].c_str());
				obj = new CPortal(x, y, r, b, scene_id);
			}
			break;*/
	case OBJECT_TYPE_WORM:
		obj = new Worm(x, y);
		break;
	case OBJECT_TYPE_JUMPER:
		obj = new Jumper(x, y);
		break;
	case OBJECT_TYPE_TELEPORTER:
		obj = new Teleporter(x, y);
		break;
	case OBJECT_TYPE_CANNON:
		obj = new Cannon(x, y);
		break;
	case OBJECT_TYPE_DOME:
		obj = new Dome(x, y);
		break;
	/*case OBJECT_TYPE_EYE:
		obj = new Eye(x, y);
		break;*/
	case OBJECT_TYPE_MINE:
		obj = new Mine(x, y);
		break;
	case OBJECT_TYPE_FLOATER:
		obj = new Floater(x, y);
		break;
	case OBJECT_TYPE_INSECT:
		obj = new Insect(x, y);
		break;
	case OBJECT_TYPE_SHIP:
		obj = new Ship(x, y);
		break;
	case OBJECT_TYPE_ORB:
		obj = new Orb(x, y);
		break;
	case OBJECT_TYPE_SKULL:
		obj = new Skull(x, y);
		break;
	case OBJECT_TYPE_SKULL_BULLET:
		obj = new SkullBullet(x, y);
		break;
	/*case OBJECT_TYPE_WALKER:
		obj = new Walker(x, y);
		break;*/
	case OBJECT_TYPE_SOPHIA:
		obj = new Sophia(x, y);
		break;
	case OBJECT_TYPE_JASON_SIDE_VIEW:
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}
	obj->SetManager(this);

	for (int i = 3; i < tokens.size(); i++)
	{
		int ani_set_id = atoi(tokens[i].c_str());
		AnimationSet *ani_set = animationSetLib->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}
	objects.insert(obj);
}
//LeSon
void SceneArea2SideView::_ParseSection_ENVIRONMENT(string line)
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
	if (tokens.size() == 7) {
		dirId = atoi(tokens[5].c_str());
		sectionToEnter = atoi(tokens[6].c_str());
	}

	GateDirection gateDir;

	Environment *env = NULL;

	switch (env_type)
	{
	case ENVIRONMENT_TYPE_WALL:
		env = new Env_Wall(x, y,width,height);
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
		else if (dirId == 3) {
			gateDir = BOTTOM;
		}
		env = new Env_Portal(x, y, width, height, gateDir, sectionToEnter);
		break;
	case ENVIRONMENT_TYPE_DUNGEON:
		if (dirId == 0) {
			gateDir = LEFT;
		}
		else {
			gateDir = RIGHT;
		}
		env = new Env_Dungeon(x, y, width, height, gateDir, sectionToEnter);
		break;
	case ENVIRONMENT_TYPE_LADDER:
		env = new Env_Ladder(x, y, width, height);
		break;
	default:
		DebugOut(L"[ERR] Invalid env type: %d\n", env_type);
		return;
	}

	environments.push_back(env);
	DebugOut(L"[INFO] An environment add type=%d, x=%f, y=%f, width=%f, height=%f\n", env_type, x, y, width, height);
}
//
//void SceneArea2SideView::_ParseSection_MAP(string line, vector<tuple<int, int, int, int, int>> &mapNav)
//{
//	unordered_map<int, MapSegment *> tempMap;
//	vector<string> tokens = split(line);
//
//	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());
//
//	if (tokens.size() < 12)
//		return; // skip invalid lines - an object set must have at least id, x, y
//
//	int id = atoi(tokens[1].c_str());
//
//	RECT boundingBox;
//	boundingBox.top = atoi(tokens[1].c_str());
//	boundingBox.left = atoi(tokens[2].c_str());
//	boundingBox.bottom = atoi(tokens[3].c_str());
//	boundingBox.right = atoi(tokens[4].c_str());
//
//	int spriteID = atoi(tokens[11].c_str());
//	const char *spriteFile = tokens[5].c_str();
//	const char *environmentFile = tokens[6].c_str();
//
//	MapSegment *mapSeg = new MapSegment(textureLib, spriteID,
//										spriteFile, environmentFile, boundingBox);
//
//	int top = atoi(tokens[7].c_str());
//	int left = atoi(tokens[8].c_str());
//	int bottom = atoi(tokens[9].c_str());
//	int right = atoi(tokens[10].c_str());
//
//	mapNav.push_back(tuple<int, int, int, int, int>{
//		id,
//		top,
//		left,
//		bottom,
//		right});
//
//	while (map.size() < id + 1)
//		map.push_back(NULL);
//	map[id] = mapSeg;
//}

void SceneArea2SideView::Init()
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
	GameGlobal::SetAnimationSetLibrary(animationSetLib);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
	//displayMessage("yeah");
}

void SceneArea2SideView::JumpCheckpoint()
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
		// section D
		else if (input[0x33]) {
			target->SetPosition(startPointInSection[3]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[3]);
		}
		// section E
		else if (input[0x34]) {
			target->SetPosition(startPointInSection[4]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[4]);
		}
		else if (input[0x35]) {
			target->SetPosition(startPointInSection[5]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[5]);
		}
		// section G
		else if (input[0x36]) {
			target->SetPosition(startPointInSection[6]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[6]);
		}
		// section H
		else if (input[0x37]) {
			target->SetPosition(startPointInSection[7]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[7]);
		}
		// section I
		else if (input[0x38]) {
			target->SetPosition(startPointInSection[8]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[8]);
		}
		// section J
		else if (input[0x39]) {
			target->SetPosition(startPointInSection[9]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[9]);
		}
		// section K
		else if (input[0x51]) {
			target->SetPosition(startPointInSection[10]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[10]);
		}
		// section L
		else if (input[0x57]) {
			target->SetPosition(startPointInSection[15]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[15]);
		}
		// section M
		else if (input[0x45]) {
			target->SetPosition(startPointInSection[16]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[16]);
		}
		// section N
		else if (input[0x52]) {
			target->SetPosition(startPointInSection[17]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[17]);
		}
		// section O
		else if (input[0x54]) {
			target->SetPosition(startPointInSection[18]);
			mCamera->SetCameraLimitarea(cameraLimitAreaOfSection[18]);
		}
	}
}

#define FRAME_PORTAL_TRANSITIONS 130
#define DISTANCE_JASON_PORTAL 90

void SceneArea2SideView::Update()
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
			}
			this->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 2);
			return;
		}
		GameGlobal::SetHealthPointSideView(target->GetHP());
		mCamera->FollowTarget();
		mCamera->SnapToBoundary();

		//LeSon
		for (auto x : onScreenObj) {
			for (auto y : environments) {
				x->Interact((Interactable*)y);
			}
		}

		for (auto object : onScreenObj)
		{
			object->Update();
		}

		// Long
		for (int i = 0; i < onScreenObj.size(); i++)
			for (int j = i + 1; j < onScreenObj.size(); j++)
				onScreenObj[i]->Interact(onScreenObj[j]);

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
			target->SetPosition(target->GetPosition() + Point(0.2, 0));
		}
		else if (directionEnterPortal == 0) {
			mCamera->SetPosition(mCamera->GetPosition() + Point(-2, 0));
			target->SetPosition(target->GetPosition() - Point(0.2, 0));
		}
		// section BF transitions
		else if (directionEnterPortal == 50) {
			mCamera->SetPosition(mCamera->GetPosition() + Point(2, 0));
			target->SetPosition(target->GetPosition() + Point(0.2, 0));
		}
		else if (directionEnterPortal == 55) {
			mCamera->SetPosition(mCamera->GetPosition() + Point(-2, 0));
			target->SetPosition(target->GetPosition() - Point(0.2, 0));
		}
		frameToTransition++;
		//DebugOut(L"Frame to transition: %d", frameToTransition);
		if (frameToTransition >= FRAME_PORTAL_TRANSITIONS) {
			if (directionEnterPortal == 1) {
				target->SetPosition(target->GetPosition() + Point(DISTANCE_JASON_PORTAL, 0));
			}
			else if (directionEnterPortal == 0) {
				target->SetPosition(target->GetPosition() - Point(DISTANCE_JASON_PORTAL, 0));
			}
			// section BF transitions
			else if (directionEnterPortal == 50) {
				target->SetPosition(Point(1586, 906));
			}
			else if (directionEnterPortal == 55) {
				target->SetPosition(Point(1488, 2954));
			}
			isCameraFree = false;
			directionEnterPortal = -1;
			frameToTransition = 0;
		}
	}

	// enter to switch scene
	if ((*input)[VK_TAB] & KEY_STATE_DOWN) {
		//Game::GetInstance()->SwitchScene(3);
		this->Release();
		Game::GetInstance()->Init(L"Resources/scene.txt", 3);
		return;
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

void SceneArea2SideView::Render()
{
	// LeSon
	int currentLivesPlay = GameGlobal::GetLivesToPlay();
	if (count < DURATION_OF_LIVESHOW && currentLivesPlay >= 0 && target==NULL)
	{
		DebugOut(L"count: %d\n", count);
		displayLivesLeft(currentLivesPlay);
		count++;
	}
	else
	{
		count = DURATION_OF_LIVESHOW + 1;
		mMap->Draw();
		for (auto object : objects)
			object->Render();
		foreMap->Draw();
		healthBar->Draw();
	}
	

	
}

/*
	Unload current scene
*/
void SceneArea2SideView::Release()
{
	for (auto object : objects)
		delete object;

	objects.clear();

	// map release sucks hihi
	mMap->Release();
	foreMap->Release();

	Scene::Release();

	healthBar->Release();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

Player* SceneArea2SideView::GetTarget()
{
	if (this == nullptr) return NULL;
	return target;
}

Camera * SceneArea2SideView::GetCamera()
{
	return mCamera;
}

void SceneArea2SideView::SetTarget(Player * player)
{
	this->target = player;
}

unordered_set<GameObject*> SceneArea2SideView::GetObjects()
{
	return objects;
}

void SceneArea2SideView::AddElement(GameObject* obj)
{
	objects.insert(obj);
}

void SceneArea2SideView::RemoveElement(GameObject * obj)
{
	toRemove.insert(obj);
}
