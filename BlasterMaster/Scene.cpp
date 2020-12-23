#include "Scene.h"
#include "BoundingBox.h"
#include "Camera.h"
#include "GameGlobal.h"

Scene::Scene(int id, LPCWSTR filePath, Game* game)
{
	this->id = id;
	this->sceneFilePath = filePath;

	this->game = game;
	textureLib = new TextureLibrary(game);
	spriteLib = new SpriteLibrary();
	animationLib = new AnimationLibrary();
	animationSetLib = new AnimationSets();
}

Scene::~Scene()
{
	textureLib->Clear();
	delete textureLib;
	spriteLib->Clear();
	delete spriteLib;
	animationLib->Clear();
	delete animationLib;
	delete animationSetLib;
}

void Scene::Release()
{
	// LeSon: maybe cannot do this, have to clear in SwitchScene for Game.cpp, discuss again hihi 
	textureLib->Clear();
	spriteLib->Clear();
	animationLib->Clear();
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

void Scene::LoadLivesLeftDisplay(TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	if (id != 2 && id != 3 && id != 5) // scene 2 sideview and 3 overhead
	{
		return;
	}
	for (size_t i = 0; i < 3; i++)
	{
		// convert str to wstr of tileset path
		std::string texturePathStr = "Resources/sprites/livesdisplay.png";
		std::wstring texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

		// health bar
		BoundingBox r = BoundingBox(i * 500, 0, (i+1)*500, 482);
		texLib->Add(LIVESHOW_TEXTURE_ID + i, texturePathWstr.c_str(), NULL);
		spriteLib->Add(LIVESHOW_TEXTURE_ID + i, r, texLib->Get(LIVESHOW_TEXTURE_ID + i));

		Sprite* sprite = spriteLib->Get(LIVESHOW_TEXTURE_ID + i);

		livesLeftDisplay.insert(std::pair<int, Sprite*>(i, sprite));
	}
}

void Scene::displayLivesLeft(int n)
{
	Point trans = Point(GameGlobal::GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
		GameGlobal::GetHeight() / 2 - Camera::GetInstance()->GetPosition().y);

	BoundingBox sourceRECT;
	Sprite* sprite = livesLeftDisplay[2-n];

	DrawArguments drawArguments;
	Point cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments.SetPosition(cameraPos + Point(-7, -24));
	drawArguments.SetTranslation(trans);
	drawArguments.SetScale(D3DXVECTOR2(0.6, 0.6));

	sprite->Draw(drawArguments);
}
