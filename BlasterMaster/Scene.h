#pragma once

#include <d3dx9.h>
#include "Input.h"

class Scene
{
protected:
	Input* input;
	int id;
	LPCWSTR sceneFilePath;

public: 
	Scene(int id, LPCWSTR filePath);

	Input* GetInput() { return input; }
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0; 
};