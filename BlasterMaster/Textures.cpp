#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>

#include "Utils.h"
#include "Game.h"
#include "textures.h"

TextureLibrary::TextureLibrary(Game* game)
{
	d3ddv = game->GetDirect3DDevice();
}

void TextureLibrary::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,			
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textureLib[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: id=%d, %s\n", id, filePath);
}

LPDIRECT3DTEXTURE9 TextureLibrary::Get(unsigned int id) 
{
	return textureLib[id];
}

/*
	Clear all loaded textures
*/
void TextureLibrary::Clear()
{
	for (auto x : textureLib)
	{
		LPDIRECT3DTEXTURE9 tex = x.second; 
		if (tex!=NULL) tex->Release();
	}
	
	textureLib.clear();
}



