#include "SceneOpening.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "GameObject.h"

#include "Worm.h"

using namespace std;

SceneOpening::SceneOpening(int id, LPCWSTR filePath, Game *game, Point screenSize) : Scene(id, filePath)
{
	this->input = game->GetInput();
	textureLib = new TextureLibrary(game);
	spriteLib = new SpriteLibrary();
	animationLib = new AnimationLibrary();
	animationSetLib = new AnimationSets();
	LoadContent();
	this->game = game;
	this->screenSize = screenSize;
	this->count = 0;
}

void SceneOpening::LoadContent()
{
	
}

SceneOpening::~SceneOpening()
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
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS 5
#define SCENE_SECTION_OBJECTS 6
#define SCENE_SECTION_MAP 7

#define OBJECT_TYPE_BOX 100

#define OBJECT_TYPE_TITLE 1
#define OBJECT_TYPE_TALE 2
#define MAX_SCENE_LINE 1024

static D3DCOLOR titleColor[4] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,0) };

void SceneOpening::Init()
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
			/*
			case SCENE_SECTION_MAP:
				_ParseSection_MAP(line, mapNav);
				break;*/
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

void SceneOpening::Update()
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

#define DURATION_OF_TITLE 500
#define DURATION_OF_TALE 1590

void SceneOpening::Render()
{
	// LeSon
	objects[0]->Render(); // bbox color
	if (count < DURATION_OF_TITLE) {
		objects[1]->Render(); // title drawing
		count++;
	}
	else {
		objects[2]->Render();
		count++;
		if (count > DURATION_OF_TALE) {
			count = 0;
		}
	}
}

void SceneOpening::_ParseSection_TEXTURES(string line)
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

void SceneOpening::_ParseSection_SPRITES(string spritePath)
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

void SceneOpening::_ParseSection_ANIMATIONS(string animationPath)
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
			//displayMessage(animationLib->Get(ani_id)->GetLoopDuration());
		}
		else
		{
			_ParseSection_ANIMATION_SETS(line);
		}
	}

	f.close();
}

void SceneOpening::_ParseSection_ANIMATION_SETS(string line)
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

void SceneOpening::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3)
		return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());


	AnimatedScene *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_TITLE:
		obj = new SceneOpeningTitle(x, y);
		break;
	case OBJECT_TYPE_TALE:
		obj = new SceneTale(x, y);
		break;
	case OBJECT_TYPE_BOX:
		obj = new SceneBox(x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	for (int i = 3; i < tokens.size(); i++)
	{
		int ani_set_id = atoi(tokens[i].c_str());
		AnimationSet *ani_set = animationSetLib->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}
	objects.push_back(obj);
}

void SceneOpening::Release()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	// LeSon: maybe cannot do this, have to clear in SwitchScene for Game.cpp, discuss again hihi 
	textureLib->Clear();
	spriteLib->Clear();
	animationLib->Clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

BoundingBox SceneOpeningTitle::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + TITLE_BBOX_WIDTH;
	float bottom = pos.y + TITLE_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void SceneOpeningTitle::Update()
{
	Input& input = *GameGlobal::GetInput();
	// enter to switch scene
	if (input[VK_RETURN]) {
		//Game::GetInstance()->SwitchScene(2);
		Game::GetInstance()->Init(L"Resources/scene.txt",2);
	}
}

void SceneOpeningTitle::Render()
{
	SetAnimationType(TITLE_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

SceneOpeningTitle::SceneOpeningTitle()
{
	SetState(TITLE_NORMAL);
}

SceneOpeningTitle::SceneOpeningTitle(float x, float y)
{
	SetState(TITLE_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1,1));
}

void SceneOpeningTitle::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case TITLE_NORMAL:
		v.x = TITLE_SPEED;
	}
}

BoundingBox SceneTale::GetBoundingBox()
{
	return BoundingBox();
}

void SceneTale::Update()
{
	Input& input = *GameGlobal::GetInput();
	// enter to switch scene
	if (input[VK_RETURN]) {
		//Game::GetInstance()->SwitchScene(2);
		Game::GetInstance()->Init(L"Resources/scene.txt", 2);
	}
}

void SceneTale::Render()
{
	SetAnimationType(TALE_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

SceneTale::SceneTale()
{
	SetState(TALE_NORMAL);
}

SceneTale::SceneTale(float x, float y)
{
	SetState(TALE_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void SceneTale::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case TALE_NORMAL:
		v.x = TALE_SPEED;
	}
}

BoundingBox SceneBox::GetBoundingBox()
{
	return BoundingBox();
}

void SceneBox::Update()
{
	drawArguments.SetColor(titleColor[(rand() % 4)]);
}

void SceneBox::Render()
{
	SetAnimationType(BOX_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

SceneBox::SceneBox()
{
	SetState(BOX_NORMAL);
}

SceneBox::SceneBox(float x, float y)
{
	SetState(BOX_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void SceneBox::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case BOX_NORMAL:
		v.x = BOX_SPEED;
	}
}
