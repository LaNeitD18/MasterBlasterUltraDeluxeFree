#include "Sprites.h"
#include "Game.h"
#include "Utils.h"

Sprite::Sprite(int id, RECT boundingBox, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->boundingBox = boundingBox;
	this->texture = tex;
}

void Sprite::Draw(Point pos, int alpha)
{
	Game * game = Game::GetInstance();
	game->Draw(pos, texture, boundingBox, alpha);
}

void SpriteLibrary::Add(int id, RECT boundingBox, LPDIRECT3DTEXTURE9 tex)
{
	Sprite* s = new Sprite(id, boundingBox, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, 
		boundingBox.left, boundingBox.top, 
		boundingBox.right, boundingBox.bottom);
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



