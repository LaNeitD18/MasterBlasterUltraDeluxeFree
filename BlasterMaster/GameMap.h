#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "MapReader/Tmx.h.in"
#include "Sprites.h"
#include "Textures.h"
#include "Camera.h"

#define SIDEVIEW_TILE_SET_ID 98765

class GameMap
{
public:
	GameMap(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib);

	Tmx::Map* GetMap();

	int GetWidth();
	int GetHeight();
	int GetTileWidth();
	int GetTileHeight();

	// co can ham update trong map khong

	void Draw();
	void Release();

	void SetCamera(Camera *camera);

	bool IsBoundLeft(); //kiem tra luc nay Camera o vi bien ben trai so voi WorldMap
	bool IsBoundRight(); // tuong tu
	bool IsBoundTop(); // tuong tu
	bool IsBoundBottom(); // tuong tu
	RECT GetWorldMapBound(); // lay bien game

	~GameMap();

private:
	void LoadMap(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib);

	bool isContain(RECT rect1, RECT rect2); // change to bounding box handle if possible

	Tmx::Map                        *mMap;
	std::map<int, Sprite*>          mListTileset;

	Camera *mCamera;
};

