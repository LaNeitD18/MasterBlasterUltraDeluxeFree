#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

#include "Worm.h"
#include "Jumper.h"
#include "Teleporter.h"

using namespace std;

SceneArea2SideView::SceneArea2SideView(int id, LPCWSTR filePath, Game* game):
	GameScene(id, filePath)
{
	this->input = game->GetInput();
	textureLib = new TextureLibrary(game);
	spriteLib = new SpriteLibrary();
	animationLib = new AnimationLibrary();
	animationSetLib = new AnimationSets();
	this->game = game;
}

SceneArea2SideView::~SceneArea2SideView()
{
	textureLib->Clear();
	delete textureLib;
	spriteLib->Clear();
	delete spriteLib;
	animationLib->Clear();
	delete animationLib;
	delete animationSetLib;
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
/*
#define OBJECT_TYPE_MARIO	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
//*/
#define OBJECT_TYPE_PORTAL	50

#define OBJECT_TYPE_WORM    4
#define OBJECT_TYPE_JUMPER  5
#define OBJECT_TYPE_TELEPORTER  6

#define MAX_SCENE_LINE 1024


void SceneArea2SideView::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	textureLib->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void SceneArea2SideView::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = textureLib->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	spriteLib->Add(ID, l, t, r, b, tex);
}

void SceneArea2SideView::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	Animation*ani = new Animation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, spriteLib, frame_time);
	}

	animationLib->Add(ani_id, ani);
}

void SceneArea2SideView::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

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
void SceneArea2SideView::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

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
	case OBJECT_TYPE_WORM: obj = new Worm(x, y); break;
	case OBJECT_TYPE_JUMPER: obj = new Jumper(x, y); break;
	case OBJECT_TYPE_TELEPORTER: obj = new Teleporter(x, y); break;
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
	}

	AnimationSet* ani_set = animationSetLib->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void SceneArea2SideView::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") { 
			section = SCENE_SECTION_SPRITES; continue; }
		if (line == "[ANIMATIONS]") { 
			section = SCENE_SECTION_ANIMATIONS; continue; }
		if (line == "[ANIMATION_SETS]") { 
			section = SCENE_SECTION_ANIMATION_SETS; continue; }
		if (line == "[OBJECTS]") { 
			section = SCENE_SECTION_OBJECTS; continue; }
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	textureLib->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

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
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void SceneArea2SideView::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	textureLib->Clear();
	spriteLib->Clear();
	animationLib->Clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
