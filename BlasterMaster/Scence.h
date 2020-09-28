#pragma once

#include <d3dx9.h>
#include "Input.h"

class GameScene
{
protected:
	Input* input;
	int id;
	LPCWSTR sceneFilePath;

public: 
	GameScene(int id, LPCWSTR filePath);

	Input* GetInput() { return input; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
};