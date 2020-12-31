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
	//int count = 50;

	// tool credit
	void init_MapLetter();
	void add_LineCredit(string line);
	void init_LetterCredit();
	void render_BBoxFollowCamera();
	void render_DragonFollowCamera();
	void render_LetterCredit();
	void move_Camera();
	bool checkValidLetter(char& ch);
};