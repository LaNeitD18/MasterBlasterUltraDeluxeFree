#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "MapReader/Tmx.h.in"
#include "Sprites.h"
#include "Textures.h"

#define HP_BBOX_WIDTH 18
#define HP_BBOX_HEIGHT 120

#define HP_TEXTURE_ID 95000

class HealthBar
{
public:
	HealthBar();
	HealthBar(TextureLibrary* texLib, SpriteLibrary* spriteLib);

	// co can ham update trong map khong

	void Draw();
	void Release();

	~HealthBar();

private:
	void Load(TextureLibrary* texLib, SpriteLibrary* spriteLib);
	std::map<int, Sprite*>		mBar;

};
