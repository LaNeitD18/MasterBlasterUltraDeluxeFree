#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "useful_stuff.h"

using namespace std;

class Sprite
{
	int id;				// Sprite ID in the sprite database

	int left; 
	int top;
	int right;
	int bottom;

	LPDIRECT3DTEXTURE9 texture;
public: 
	Sprite(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	
	void Draw(Point pos, int alpha = 255);
};

/*
	Manage sprite database
*/
class SpriteLibrary
{
	static SpriteLibrary * __instance;

	unordered_map<int, Sprite*> sprites;

public:
	void Add(int id, int left, int top, int right, int bottom, LPDIRECT3DTEXTURE9 tex);
	Sprite* Get(int id);
	void Clear();

	static SpriteLibrary * GetInstance();
};



