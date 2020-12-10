#pragma once
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "GameMap.h"
#include "AnimatedGameObject.h"

class SceneOpening : public Scene
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

public:
	SceneOpening(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneOpening();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	// state for entering sophia
	int enterState; // title with tale = 0, entering sophia = 1, enter = 2

	// time before render tale
	int count = 50;
};

#define TITLE_SPEED 0.00f;

#define TITLE_BBOX_WIDTH 800
#define TITLE_BBOX_HEIGHT 600

#define TITLE_NORMAL 0

class SceneOpeningTitle : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	SceneOpeningTitle();
	SceneOpeningTitle(float x, float y);
	virtual void SetState(int state);
};

#define TALE_SPEED 0.01f;

#define TALE_BBOX_WIDTH 800
#define TALE_BBOX_HEIGHT 600

#define TALE_NORMAL 0

class SceneTale : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
public:
	SceneTale();
	SceneTale(float x, float y);
	virtual void SetState(int state);
};

#define BOX_SPEED 0.01f;

#define BOX_BBOX_WIDTH 800
#define BOX_BBOX_HEIGHT 600

#define BOX_NORMAL 0

class SceneBox : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
public:
	SceneBox();
	SceneBox(float x, float y);
	virtual void SetState(int state);
};

class SceneBox1 : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
public:
	SceneBox1();
	SceneBox1(float x, float y);
	virtual void SetState(int state);
};

#define ENTER_SPEED 0.01f;

#define ENTER_BBOX_WIDTH 800
#define ENTER_BBOX_HEIGHT 600

#define ENTER_NORMAL 0

class SceneEnter : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
public:
	SceneEnter();
	SceneEnter(float x, float y);
	virtual void SetState(int state);
};

