#pragma once

#include <d3dx9.h>
#include "Input.h"
#include <vector>
#include "Sprites.h"
#include "Textures.h"
#include "Animations.h"

#define DURATION_OF_LIVESHOW 110

#define LIVESHOW_TEXTURE_ID 94500


class Scene
{
protected:
	Input* input;
	int id;
	LPCWSTR sceneFilePath;

	//LeSon
	bool isCameraFree; // used for go to portal
	int directionEnterPortal; // 0 portal left 1 portal right 2 portal top 3 portal bottom -1 normal
	int frameToTransition;

	Game* game;

	TextureLibrary* textureLib;
	SpriteLibrary* spriteLib;
	AnimationLibrary* animationLib;
	AnimationSets* animationSetLib;

	std::unordered_map<int, Sprite*>		livesLeftDisplay;

	int count = 50;

public: 
	Scene(int id, LPCWSTR filePath, Game* game);
	~Scene();

	Input* GetInput() { return input; }
	virtual void Init() = 0;
	virtual void Release();
	virtual void Update() = 0;
	virtual void Render() = 0;

	//LeSon
	bool CheckFreeCamera();
	void SetFreeCamera(bool isFree);
	int GetDirectionEnter();
	void SetDirectionEnter(int dir);

	void LoadLivesLeftDisplay(TextureLibrary* texLib, SpriteLibrary* spriteLib);
	void displayLivesLeft(int n);
	
};