#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "GameMap.h"

class SceneArea2Overhead : public Scene
{
protected:
	//CMario *player;					// A play scene has to have player, right? 

	vector<GameObject*> objects;

	TextureLibrary* textureLib;
	SpriteLibrary* spriteLib;
	AnimationLibrary* animationLib;
	AnimationSets* animationSetLib;
	GameMap* mMap;

	void LoadContent();

	// camera
	Camera *mCamera;
private:
	Game* game;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	//void _ParseSection_MAP(string line, vector<tuple<int, int, int, int, int> > &mapNav);

	Point screenSize;
public:
	SceneArea2Overhead(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneArea2Overhead();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();
};
