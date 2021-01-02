#pragma once
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "GameMap.h"
#include "AnimatedGameObject.h"

class Letter {
public:
	Point pos;
	char letter;
	Letter(Point pos, char ch)
	{
		this->pos = pos;
		this->letter = ch;
	}
};

#define DRAGON_BBOX_WIDTH 800
#define DRAGON_BBOX_HEIGHT 600

#define DRAGON_NORMAL 0

class Dragon : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Dragon();
	Dragon(float x, float y);
	virtual void SetState(int state);
};

class Credit : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Credit();
	Credit(float x, float y);
	virtual void SetState(int state);
};

class SceneEnd : public Scene
{
protected:
	vector<GameObject*> objects;

	void LoadContent();

private:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	Point screenSize;
	map<char, int> mapLetter;
	vector <Letter> Paragraph;
	Camera* mCamera;
	Point posLetter;

public:
	SceneEnd(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneEnd();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	// state for entering sophia
	//int enterState; // title with tale = 0, entering sophia = 1, enter = 2

	// time before render tale
	int count = 50;

	// tool credit
	void init_MapLetter();
	void add_LineCredit(string line);
	void init_LetterCredit();
	void render_BBox1();
	void render_DragonFollowCamera();
	void render_LetterCredit();
	void move_Camera_Up();
	bool checkValidLetter(char& ch);

	int shakeState;

	void render_BBox2();
	void render_Begin();
	void render_MountItem();
	void render_Background();
	void move_Camera_Right();
};

#define BEGIN_BBOX_WIDTH 800
#define BEGIN_BBOX_HEIGHT 600

#define BEGIN_NORMAL 0

class Begin : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Begin();
	Begin(float x, float y);
	virtual void SetState(int state);
};

#define MT_SPEED 0.00f;

#define MT_BBOX_WIDTH 30
#define MT_BBOX_HEIGHT 30

#define MT_NORMAL 0

class MountainItem : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	MountainItem();
	MountainItem(float x, float y);
	virtual void SetState(int state);
};

#define BACKGROUND_SPEED 0.00f;

#define BACKGROUND_BBOX_WIDTH 30
#define BACKGROUND_BBOX_HEIGHT 30

#define BACKGROUND_NORMAL 0

class Background : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Background();
	Background(float x, float y);
	virtual void SetState(int state);
};

class SceneBox2 : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
public:
	SceneBox2();
	SceneBox2(float x, float y);
	virtual void SetState(int state);
};
