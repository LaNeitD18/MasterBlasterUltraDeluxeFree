#include "SceneOver.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "GameObject.h"
#include "GameGlobal.h"

using namespace std;

SceneOver::SceneOver(int id, LPCWSTR filePath, Game* game, Point screenSize) : Scene(id, filePath, game)
{
	this->input = game->GetInput();
	LoadContent();
	this->screenSize = screenSize;
	this->count = 0;
	this->selection = 0;

	GameGlobal::SetAnimationSetLibrary(animationSetLib);
}

void SceneOver::LoadContent()
{

}

SceneOver::~SceneOver()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();
}

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS 5
#define SCENE_SECTION_OBJECTS 6
#define SCENE_SECTION_MAP 7

#define OBJECT_TYPE_OVER 1
#define OBJECT_TYPE_SELECTION 2

#define MAX_SCENE_LINE 1024

void SceneOver::Init()
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

void SceneOver::Select() {
	Point pos = objects[1]->GetPosition();
	if (((*input)[VK_UP] & KEY_STATE_DOWN) && selection == 1) {
		selection = 0;
		objects[1]->SetPosition(Point(pos.x, pos.y - 30));
	}
	else if (((*input)[VK_DOWN] & KEY_STATE_DOWN) && selection == 0) {
		selection = 1;
		objects[1]->SetPosition(Point(pos.x, pos.y + 30));
	}
	else if ((*input)[VK_SPACE] & KEY_STATE_DOWN) {
		if (selection == 1) {
			this->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 1);
			return;
		}
		else {
			this->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 2);
			return;
		}
	}
}

void SceneOver::Update()
{
	input->Update();
	Select();
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

void SceneOver::Render()
{
	// LeSon
	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Render();
	}
}

void SceneOver::_ParseSection_TEXTURES(string line)
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

void SceneOver::_ParseSection_SPRITES(string spritePath)
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

void SceneOver::_ParseSection_ANIMATIONS(string animationPath)
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
			//displayMessage(animationLib->Get(ani_id)->GetLoopDuration());
		}
		else
		{
			_ParseSection_ANIMATION_SETS(line);
		}
	}

	f.close();
}

void SceneOver::_ParseSection_ANIMATION_SETS(string line)
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

void SceneOver::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3)
		return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());


	AnimatedScene* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_OVER:
		obj = new SceneOverContinue(x, y);
		break;
	case OBJECT_TYPE_SELECTION:
		obj = new RoundSelection(x, y);
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	for (int i = 3; i < tokens.size(); i++)
	{
		int ani_set_id = atoi(tokens[i].c_str());
		AnimationSet* ani_set = animationSetLib->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
	}
	objects.push_back(obj);
}

void SceneOver::Release()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

BoundingBox SceneOverContinue::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + CONTINUE_BBOX_WIDTH;
	float bottom = pos.y + CONTINUE_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void SceneOverContinue::Update()
{
	Input& input = *GameGlobal::GetInput();
	//SceneOver* scene = dynamic_cast<SceneOver*>(Game::GetInstance()->GetCurrentScene());
	//// enter to switch scene
	//if ((input[VK_RETURN] & KEY_STATE_DOWN) && scene->enterState == 0) {
	//	scene->count = 0;
	//	scene->enterState = 1;
	//}
}

void SceneOverContinue::Render()
{
	SetAnimationType(CONTINUE_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

SceneOverContinue::SceneOverContinue()
{
	SetState(CONTINUE_NORMAL);
}

SceneOverContinue::SceneOverContinue(float x, float y)
{
	SetState(CONTINUE_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void SceneOverContinue::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case CONTINUE_NORMAL:
		v.x = CONTINUE_SPEED;
	}
}

BoundingBox RoundSelection::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + SELECTION_BBOX_WIDTH;
	float bottom = pos.y + SELECTION_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void RoundSelection::Update()
{
	Input& input = *GameGlobal::GetInput();
	//SceneOver* scene = dynamic_cast<SceneOver*>(Game::GetInstance()->GetCurrentScene());
	//// enter to switch scene
	//if ((input[VK_RETURN] & KEY_STATE_DOWN) && scene->enterState == 0) {
	//	scene->count = 0;
	//	scene->enterState = 1;
	//}
}

void RoundSelection::Render()
{
	SetAnimationType(SELECTION_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

RoundSelection::RoundSelection()
{
	SetState(SELECTION_NORMAL);
}

RoundSelection::RoundSelection(float x, float y)
{
	SetState(SELECTION_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void RoundSelection::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case SELECTION_NORMAL:
		v.x = SELECTION_SPEED;
	}
}
