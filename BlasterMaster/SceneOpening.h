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

	TextureLibrary* textureLib;
	SpriteLibrary* spriteLib;
	AnimationLibrary* animationLib;
	AnimationSets* animationSetLib;

	void LoadContent();

private:
	Game* game;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	Point screenSize;

	int count = 50;

public:
	SceneOpening(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneOpening();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();
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
