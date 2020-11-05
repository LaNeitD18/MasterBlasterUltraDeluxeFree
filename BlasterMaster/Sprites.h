#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>
#include "useful_stuff.h"
#include "GameGlobal.h"
#include "DrawArguments.h"

using namespace std;

class Sprite
{
	int id;				// Sprite ID in the sprite database
	
public: 
	Sprite(int id, RECT boundingBox, LPDIRECT3DTEXTURE9 tex);

	void SetWidth(int width);
	int GetWidth();

	void SetHeight(int height);
	int GetHeight();

	void SetSourceRect(RECT rect);

	
	//void Draw(Point pos, int alpha = 255);
	void Draw(DrawArguments drawArguments);

protected:
	LPDIRECT3DTEXTURE9      mTexture; // load hinh anh vao day
	LPD3DXSPRITE            mSpriteHandler; //SpriteHandler ho tro ve hinh
	D3DXIMAGE_INFO          mImageInfo; // thong tin thuc cua hinh anh duoc lay
	RECT                    mSpriteRect; // hinh chu nhat cat tu anh cua texture
	int                     mWidth, mHeight; // kich thuoc cua texture
	D3DXMATRIX              mMatrix; // ma tran cua Spite ho tro trong cac phep hinh hoc

	//su dung cho ke thua
	void Init();

	LPDIRECT3DTEXTURE9 GetTexture();
	D3DXIMAGE_INFO GetImageInfo(); // lay thong tin thuc cua hinh anh duoc lay
	

	bool isRect(RECT rect);

	
};

/*
	Manage sprite database
*/
class SpriteLibrary
{
	unordered_map<int, Sprite*> sprites;

public:
	void Add(int id, RECT boundingBox, LPDIRECT3DTEXTURE9 tex);
	Sprite* Get(int id);
	void Clear();
};