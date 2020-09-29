#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"


class SceneArea2SideView: public GameScene
{
protected: 
	//CMario *player;					// A play scene has to have player, right? 

	vector<GameObject*> objects;
	TextureLibrary* textureLib;
	SpriteLibrary* spriteLib;
	AnimationLibrary* animationLib;
	AnimationSets* animationSetLib;
private:
	Game* game;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

	
public: 
	SceneArea2SideView(int id, LPCWSTR filePath, Game* game);
	virtual ~SceneArea2SideView();

	virtual void Load();
	virtual void Update();
	virtual void Render();
	virtual void Unload();
};
