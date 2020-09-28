#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

Sprite::Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->left = left;
	this->top = top;
	this->right = right;
	this->bottom = bottom;
	this->texture = tex;
}

SpriteLibrary * SpriteLibrary::__instance = NULL;

SpriteLibrary *SpriteLibrary::GetInstance()
{
	if (__instance == NULL) __instance = new SpriteLibrary();
	return __instance;
}

void Sprite::Draw(Point pos, int alpha)
{
	Game * game = Game::GetInstance();
	RECT rect;
	rect.left = left;
	rect.right = right;
	rect.top = top;
	rect.bottom = bottom;
	game->Draw(pos, texture, rect, alpha);
}

void SpriteLibrary::Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex)
{
	Sprite* s = new Sprite(id, left, top, right, bottom, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, left, top, right, bottom);
}

Sprite* SpriteLibrary::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void SpriteLibrary::Clear()
{
	for (auto x : sprites)
	{
		Sprite* s = x.second;
		delete s;
	}

	sprites.clear();
}



