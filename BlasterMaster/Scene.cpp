#include "Scene.h"

Scene::Scene(int id, LPCWSTR filePath)
{
	this->id = id;
	this->sceneFilePath = filePath;
}

bool Scene::CheckFreeCamera()
{
	return isCameraFree;
}

void Scene::SetFreeCamera(bool isFree)
{
	isCameraFree = isFree;
}

int Scene::GetDirectionEnter()
{
	return directionEnterPortal;
}

void Scene::SetDirectionEnter(int dir)
{
	directionEnterPortal = dir;
}
