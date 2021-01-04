#include "SceneEnd.h"
#include <iostream>
#include <fstream>

#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "GameObject.h"
#include "GameGlobal.h"
#include "SceneBoss.h"
#include "SceneOpening.h"
#include "Sound.h"
#include "Worm.h"
#include <algorithm>

using namespace std;

SceneEnd::SceneEnd(int id, LPCWSTR filePath, Game* game, Point screenSize) : Scene(id, filePath, game)
{
	this->input = game->GetInput();
	LoadContent();
	this->screenSize = screenSize;
	this->count = 0;
	//this->enterState = 0;

	GameGlobal::SetAnimationSetLibrary(animationSetLib);
}

void SceneEnd::LoadContent()
{
	// camera setup
	mCamera = new Camera(Point(GameGlobal::GetWidth(), GameGlobal::GetHeight()));
	Camera::setCameraInstance(mCamera);
}

SceneEnd::~SceneEnd()
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

#define OBJECT_TYPE_BBOX1 505
#define OBJECT_TYPE_BBOX2 506

#define OBJECT_TYPE_BOX 100

#define OBJECT_TYPE_BACKGROUND 3401
#define OBJECT_TYPE_MOUNTAIN 3402
#define OBJECT_TYPE_BEGIN 3403
#define OBJECT_TYPE_CREDIT	3404
#define OBJECT_TYPE_DRAGON	3406

#define MAX_SCENE_LINE		1024


static D3DCOLOR titleColor[4] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,0) };

void SceneEnd::Init()
{
	//vector<tuple<int, int, int, int, int>> mapNav;
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);
	Sound::getInstance()->play("intro", false, 1);

	// move bbox init
	textureLib->Add(ID_TEX_BBOX, L"Resources\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	init_MapLetter();
	init_LetterCredit();

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
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void SceneEnd::Update()
{
	input->Update();
	if (count > 150000) {
		count = 0;
	}
	count++;
	Point camPos = Camera::GetInstance()->GetPosition();
	//DebugOut(L"x%f, y%f", camPos.x, camPos.y);

	//// test continue game
	//if ((*input)[VK_BACK] && KEY_STATE_DOWN) {
	//	this->Release();
	//	Game::GetInstance()->Init(L"Resources/scene.txt", 5);
	//	SceneBoss* scene = dynamic_cast<SceneBoss*>(Game::GetInstance()->GetCurrentScene());
	//	scene->liveShow = 0;
	//	return;
	//}

	//if (enterState == 2) {
	//	//Game::GetInstance()->SwitchScene(2);
	//	this->Release();
	//	Game::GetInstance()->Init(L"Resources/scene.txt", 2);
	//	return;
	//}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update();
	}

	//// Update camera to follow mario
	//Point pos;
	///*
	//pos.x -= game->GetScreenWidth() / 2;
	//pos.y -= game->GetScreenHeight() / 2;
	////*/
	//game->SetCamPos(pos);
}

#define DURATION_OF_BEGIN 1500
#define START_POINT_SHAKE 500
#define FINISH_POINT_SHAKE 1200
#define DURATION_OF_BACKGROUND 3000
#define START_POINT_MOVERIGHT 1800

void SceneEnd::Render()
{
	if (count == 1) {
		Sound::getInstance()->stop();
	}
	render_BBox2();
	if (count < DURATION_OF_BEGIN) {
		
		render_MountItem();
		render_Begin();
		DebugOut(L"count %d", count);
		if (count == START_POINT_SHAKE + 1) {
			Sound::getInstance()->play("fire", false, 1);
		}
		if (count == (FINISH_POINT_SHAKE - 1)) {
			Sound::getInstance()->stop();
		}
		if (count >= START_POINT_SHAKE && count < FINISH_POINT_SHAKE) {
			shakeState = 1;
		}
		else {
			shakeState = 0;
		}
		
	}
	else if (count < DURATION_OF_BACKGROUND) {
		render_BBox1();
		render_Background();
		if (count == START_POINT_MOVERIGHT + 1) {
			Sound::getInstance()->play("fire", false, 1);
		}
		if (count > START_POINT_MOVERIGHT) {
			move_Camera_Right();
		}
	}
	else if (count < 3010) {
		render_BBox1();
		Camera::GetInstance()->SetPosition(Point(0, 0));
	}
	else {
		if (count == DURATION_OF_BACKGROUND + 21) {
			Sound::getInstance()->stop();
			Sound::getInstance()->play("enter", false, 1);
		}
		render_BBox1();
		move_Camera_Up();
		render_DragonFollowCamera();
		render_LetterCredit();
	}
}

void SceneEnd::_ParseSection_TEXTURES(string line)
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

void SceneEnd::_ParseSection_SPRITES(string spritePath)
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

void SceneEnd::_ParseSection_ANIMATIONS(string animationPath)
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

void SceneEnd::_ParseSection_ANIMATION_SETS(string line)
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

void SceneEnd::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_BEGIN:
		obj = new Begin(x, y);
		break;
	case OBJECT_TYPE_BACKGROUND:
		obj = new Background(x, y);
		break;
	case OBJECT_TYPE_MOUNTAIN:
		obj = new MountainItem(x, y);
		break;
	case OBJECT_TYPE_CREDIT:
		obj = new Credit(x, y);
		break;
	case OBJECT_TYPE_DRAGON:
		obj = new Dragon(x, y);
		break;
	case OBJECT_TYPE_BBOX1:
		obj = new SceneBox1(x, y);
		break;
	case OBJECT_TYPE_BBOX2:
		obj = new SceneBox2(x, y);
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

void SceneEnd::Release()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

//Tool Credit
void SceneEnd::init_MapLetter()
{
	mapLetter['!'] = 3310;
	mapLetter['0'] = 3311;
	mapLetter['1'] = 3312;
	mapLetter['2'] = 3313;
	mapLetter['3'] = 3314;
	mapLetter['4'] = 3315;
	mapLetter['5'] = 3316;
	mapLetter['6'] = 3317;
	mapLetter['7'] = 3318;
	mapLetter['8'] = 3319;
	mapLetter['9'] = 3320;
	mapLetter['a'] = 3321;
	mapLetter['b'] = 3322;
	mapLetter['c'] = 3323;
	mapLetter['d'] = 3324;
	mapLetter['e'] = 3325;
	mapLetter['f'] = 3326;
	mapLetter['g'] = 3327;
	mapLetter['h'] = 3328;
	mapLetter['i'] = 3329;
	mapLetter['j'] = 3330;
	mapLetter['k'] = 3331;
	mapLetter['l'] = 3332;
	mapLetter['m'] = 3333;
	mapLetter['n'] = 3334;
	mapLetter['o'] = 3335;
	mapLetter['p'] = 3336;
	mapLetter['q'] = 3337;
	mapLetter['r'] = 3338;
	mapLetter['s'] = 3339;
	mapLetter['t'] = 3340;
	mapLetter['u'] = 3341;
	mapLetter['v'] = 3342;
	mapLetter['w'] = 3343;
	mapLetter['x'] = 3344;
	mapLetter['y'] = 3345;
	mapLetter['z'] = 3346;
}

void SceneEnd::init_LetterCredit()
{
	Camera::GetInstance()->SetPosition(Point(0, 0));
	posLetter = Point(-40, 100);
	ifstream f;
	f.open(L"Resources/credit.txt");

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);
		add_LineCredit(line);
	}

	f.close();

	DebugOut(L"\n[INFO] Done loading CREDIT resources");
}

void SceneEnd::add_LineCredit(string line)
{
	posLetter.x = -40;
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	for (int i = 0; i < line.length(); i++)
	{
		char ch = line[i];
		if (ch == ' ')
			posLetter.x += 5;
		else
			if (!checkValidLetter(ch))
				posLetter.x += 10;
			else
			{
				posLetter.x += 10;
				//DebugOut(L"\n lx %f ly %f", posLetter.x, posLetter.y);
				Letter item(posLetter, ch);
				Paragraph.push_back(item);
			}
	}
	posLetter.y += 10;
}

void SceneEnd::render_LetterCredit()
{
	for (int i = 0; i < Paragraph.size(); i++)
	{
		Letter item = Paragraph[i];
		int id = mapLetter[item.letter];
		//Credit[id]->Render(item.x, item.y);

		for (auto x : objects) {
			bool isCredit = dynamic_cast<Credit*>(x) != NULL;
			if (isCredit) {
				AnimationSet* ani_set = animationSetLib->Get(id);
				x->SetAnimationSet(ani_set);
				x->SetPosition(item.pos);
				x->Render();
			}
		}
	}
}

void SceneEnd::render_BBox2()
{
	Point cameraPos = Camera::GetInstance()->GetPosition();

	for (auto x : objects) {
		bool isBBox = dynamic_cast<SceneBox2*>(x) != NULL;
		if (isBBox) {
			x->SetPosition(cameraPos);
			x->Render();
		}
	}
}

void SceneEnd::render_BBox1()
{
	Point cameraPos = Camera::GetInstance()->GetPosition();

	for (auto x : objects) {
		bool isBBox = dynamic_cast<SceneBox1*>(x) != NULL;
		if (isBBox) {
			x->SetPosition(cameraPos);
			x->Render();
		}
	}
}

void SceneEnd::render_DragonFollowCamera()
{
	Point cameraPos = Camera::GetInstance()->GetPosition();
	cameraPos.x -= 20;
	cameraPos.y -= 10;
	for (auto x : objects) {
		bool isDragon = dynamic_cast<Dragon*>(x) != NULL;
		if (isDragon) {
			x->SetPosition(cameraPos);
			x->Render();
		}
	}
}

void SceneEnd::move_Camera_Up()
{
	Point camPos = Camera::GetInstance()->GetPosition();
	if (camPos.y >= 210)
		return;
	camPos.y += 0.5;
	Camera::GetInstance()->SetPosition(camPos);
}

void SceneEnd::move_Camera_Right()
{
	Point camPos = Camera::GetInstance()->GetPosition();
	if (camPos.x > 254)
		return;
	camPos.x += 0.5;
	Camera::GetInstance()->SetPosition(camPos);
}

bool SceneEnd::checkValidLetter(char& ch)
{
	if (ch >= 'a' && ch <= 'z')
		return true;
	if (ch >= '0' && ch <= '9')
		return true;
	return false;
}

void SceneEnd::render_Begin()
{
	for (auto x : objects) {
		bool isBegin = dynamic_cast<Begin*>(x) != NULL;
		if (isBegin) {
			Point pos = x->GetPosition();
			if (shakeState == 1) {
				if (count % 5 == 0) {
					x->SetPosition(Point(-8, -18));
				}
				else {
					x->SetPosition(Point(-8, -14));
				}
			}
			x->Render();
		}
	}
}

void SceneEnd::render_MountItem()
{
	for (auto x : objects) {
		bool isMT = dynamic_cast<MountainItem*>(x) != NULL;
		if (isMT) {
			Point pos = x->GetPosition();
			if (shakeState == 1) {
				if (count % 5 == 0) {
					x->SetPosition(pos + Point(0, 0.3));
				}
			}
			x->Render();
		}
	}
}

void SceneEnd::render_Background()
{
	Point cameraPos = Camera::GetInstance()->GetPosition();
	for (auto x : objects) {
		bool isBackground = dynamic_cast<Background*>(x) != NULL;
		if (isBackground) {
			//x->SetPosition(cameraPos);
			x->Render();
		}
	}
}

// Dragon
BoundingBox Dragon::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + DRAGON_BBOX_WIDTH;
	float bottom = pos.y + DRAGON_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Dragon::Update()
{
	//DebugOut(L"pos x%f y%f", pos.x, pos.y);
	Input& input = *GameGlobal::GetInput();
}

void Dragon::Render()
{
	SetAnimationType(DRAGON_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

Dragon::Dragon()
{
	SetState(DRAGON_NORMAL);
}

Dragon::Dragon(float x, float y)
{
	SetState(DRAGON_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void Dragon::SetState(int state)
{
	GameObject::SetState(state);
}


// Credit
BoundingBox Credit::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + DRAGON_BBOX_WIDTH;
	float bottom = pos.y + DRAGON_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Credit::Update()
{
	//DebugOut(L"pos x%f y%f", pos.x, pos.y);
	Input& input = *GameGlobal::GetInput();
	SceneEnd* scene = dynamic_cast<SceneEnd*>(Game::GetInstance()->GetCurrentScene());
	//// enter to switch scene
	//if ((input[VK_RETURN] & KEY_STATE_DOWN) && scene->enterState == 0) {
	//	Sound::getInstance()->stop("intro");
	//	Sound::getInstance()->play("enter", false, 1);
	//	scene->count = 0;
	//	scene->enterState = 1;
	//}
}

void Credit::Render()
{
	SetAnimationType(DRAGON_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

Credit::Credit()
{
	SetState(DRAGON_NORMAL);
}

Credit::Credit(float x, float y)
{
	SetState(DRAGON_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void Credit::SetState(int state)
{
	GameObject::SetState(state);
}

// begin
BoundingBox Begin::GetBoundingBox()
{
	return BoundingBox();
}

void Begin::Update()
{
	//DebugOut(L"pos x%f y%f", pos.x, pos.y);
	Input& input = *GameGlobal::GetInput();

}

void Begin::Render()
{
	SetAnimationType(DRAGON_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

Begin::Begin()
{
	SetState(BEGIN_NORMAL);
}

Begin::Begin(float x, float y)
{
	SetState(BEGIN_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void Begin::SetState(int state)
{
	GameObject::SetState(state);
}

// mountain item
BoundingBox MountainItem::GetBoundingBox()
{
	return BoundingBox();
}

void MountainItem::Update()
{

}

void MountainItem::Render()
{
	SetAnimationType(MT_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

MountainItem::MountainItem()
{
	SetState(MT_NORMAL);
}

MountainItem::MountainItem(float x, float y)
{
	SetState(MT_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void MountainItem::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MT_NORMAL:
		v.x = MT_SPEED;
	}
}

BoundingBox Background::GetBoundingBox()
{
	return BoundingBox();
}

void Background::Update()
{
	
}

void Background::Render()
{
	SetAnimationType(BACKGROUND_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

Background::Background()
{
	SetState(BACKGROUND_NORMAL);
}

Background::Background(float x, float y)
{
	SetState(BACKGROUND_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void Background::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case BACKGROUND_NORMAL:
		v.x = BACKGROUND_SPEED;
	}
}

// bbox2
BoundingBox SceneBox2::GetBoundingBox()
{
	return BoundingBox();
}

void SceneBox2::Update()
{
	//drawArguments.SetColor(titleColor[(rand() % 4)]);
}

void SceneBox2::Render()
{
	SetAnimationType(BOX_NORMAL);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

SceneBox2::SceneBox2()
{
	SetState(BOX_NORMAL);
}

SceneBox2::SceneBox2(float x, float y)
{
	SetState(BOX_NORMAL);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

void SceneBox2::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case BOX_NORMAL:
		v.x = BOX_SPEED;
	}
}