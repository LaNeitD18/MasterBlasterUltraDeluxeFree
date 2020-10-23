#include <iostream>
#include <fstream>

#include "SceneArea2SideView.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

#include "Worm.h"
#include "Jumper.h"
#include "Teleporter.h"
#include "Cannon.h"
#include "Dome.h"
#include "Eye.h"
#include "Laser.h"
#include "Mine.h"
#include "Floater.h"
#include "WormPod.h"
#include "Insect.h"
#include "Orb.h"
#include "Walker.h"

using namespace std;

SceneArea2SideView::SceneArea2SideView(int id, LPCWSTR filePath, Game *game, Point screenSize) : Scene(id, filePath)
{
	this->input = game->GetInput();
	textureLib = new TextureLibrary(game);
	spriteLib = new SpriteLibrary();
	animationLib = new AnimationLibrary();
	animationSetLib = new AnimationSets();
	LoadContent();
	//mMap = new GameMap("Map/General/level2-side-tiless.tmx", textureLib, spriteLib);
	this->game = game;
	this->screenSize = screenSize;
}

void SceneArea2SideView::LoadContent()
{
	mMap = new GameMap("Map/General/level2-side-tiless.tmx", textureLib, spriteLib);

	// camera setup
	mCamera = new Camera(Point(GameGlobal::GetWidth(), GameGlobal::GetHeight()));
	mCamera->SetPosition(GameGlobal::GetWidth() / 2,
		GameGlobal::GetHeight() / 2);
	mMap->SetCamera(mCamera);
}

SceneArea2SideView::~SceneArea2SideView()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
	textureLib->Clear();
	delete textureLib;
	spriteLib->Clear();
	delete spriteLib;
	animationLib->Clear();
	delete animationLib;
	delete animationSetLib;
	mMap->Release();
	delete mMap;
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
#define OBJECT_TYPE_PORTAL 50

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

			RECT bbox;
			bbox.left = atoi(tokens[1].c_str());
			bbox.top = atoi(tokens[2].c_str());
			bbox.right = atoi(tokens[3].c_str());
			bbox.bottom = atoi(tokens[4].c_str());
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

	int ani_set_id = atoi(tokens[3].c_str());

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
	case OBJECT_TYPE_EYE:
		obj = new Eye(x, y);
		break;
	case OBJECT_TYPE_MINE:
		obj = new Mine(x, y);
		break;
	case OBJECT_TYPE_FLOATER:
		obj = new Floater(x, y);
		break;
	case OBJECT_TYPE_INSECT:
		obj = new Insect(x, y);
		break;
	case OBJECT_TYPE_ORB:
		obj = new Orb(x, y);
		break;
	case OBJECT_TYPE_WALKER:
		obj = new Walker(x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	AnimationSet *ani_set = animationSetLib->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
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
		}/*
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
			break;/*
		case SCENE_SECTION_MAP:
			_ParseSection_MAP(line, mapNav);
			break;*/
		}
	}

	f.close();

	textureLib->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

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
}

void SceneArea2SideView::Update()
{
	input->Update();

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}

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
	/*for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();*/
	// LeSon
	mMap->Draw();
}

/*
	Unload current scene
*/
void SceneArea2SideView::Release()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	mMap->Release();

	textureLib->Clear();
	spriteLib->Clear();
	animationLib->Clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
