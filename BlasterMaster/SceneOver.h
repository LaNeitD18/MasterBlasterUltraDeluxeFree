#pragma once
#include "Scene.h"
#include "Game.h"
#include "Textures.h"
#include "GameObject.h"
#include "GameMap.h"
#include "AnimatedGameObject.h"

class SceneOver : public Scene
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
	int selection;

public:
	SceneOver(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneOver();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	void Select();

};

#define CONTINUE_SPEED 0.00f;

#define CONTINUE_BBOX_WIDTH 800
#define CONTINUE_BBOX_HEIGHT 600

#define CONTINUE_NORMAL 0

class SceneOverContinue : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	SceneOverContinue();
	SceneOverContinue(float x, float y);
	virtual void SetState(int state);
};

#define SELECTION_SPEED 0.00f;

#define SELECTION_BBOX_WIDTH 30
#define SELECTION_BBOX_HEIGHT 30

#define SELECTION_NORMAL 0

class RoundSelection : public AnimatedScene
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	RoundSelection();
	RoundSelection(float x, float y);
	virtual void SetState(int state);
};
