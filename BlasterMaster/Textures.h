#pragma once
#include <unordered_map>
#include <d3dx9.h>
class Game;

using std::unordered_map;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;

	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

	LPDIRECT3DDEVICE9 d3ddv;

public: 
	CTextures(Game* game);
	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	void Clear();
};