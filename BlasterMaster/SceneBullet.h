#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "Sprites.h"
#include <map>
#include "Textures.h"

#define BULLET_SCENE_BBOX_WIDTH 18
#define BULLET_SCENE_BBOX_HEIGHT 120

#define BULLET_SCENE_TEXTURE_ID 96000
#define NUMS_TEXTURE_ID 97000
#define BUTTON_TEXTURE_ID 98000

class SceneBullet
{
public:
	SceneBullet();
	SceneBullet(TextureLibrary* texLib, SpriteLibrary* spriteLib);

	void SelectType();

	void Draw();
	void Release();

	~SceneBullet();

private:
	void Load(TextureLibrary* texLib, SpriteLibrary* spriteLib);
	Sprite* selection;
	std::map<int, Sprite*>		nums;
	Sprite* base;

};
