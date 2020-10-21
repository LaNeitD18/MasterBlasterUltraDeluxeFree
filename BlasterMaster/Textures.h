#pragma once
#include <unordered_map>
#include <d3dx9.h>
class Game;

using std::unordered_map;

/*
	Manage texture database
*/
class TextureLibrary
{
	//static TextureLibrary * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textureLib;

	LPDIRECT3DDEVICE9 d3ddv;

public: 
	TextureLibrary(Game* game);
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
};