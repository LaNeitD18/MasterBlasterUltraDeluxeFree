#pragma once

#include <d3dx9.h>
#include "Input.h"

class Scene
{
protected:
	Input* input;
	int id;
	LPCWSTR sceneFilePath;

	//LeSon
	bool isCameraFree; // used for go to portal
	int directionEnterPortal; // 0 portal left 1 portal right -1 normal
	int frameToTransition;


public: 
	Scene(int id, LPCWSTR filePath);

	Input* GetInput() { return input; }
	virtual void Init() = 0;
	virtual void Release() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	//LeSon
	bool CheckFreeCamera();
	void SetFreeCamera(bool isFree);
	int GetDirectionEnter();
	void SetDirectionEnter(int dir);
	
};