#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>


#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define DIRECTINPUT_VERSION 0x0800

#include "useful_stuff.h"
#include "Scene.h"
#include "Input.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class Game
{
	static Game * __instance;
	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;		
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	Point cameraPosition;


	unordered_map<int, Scene*> scenes;
	int current_scene; 

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

	Input* input = NULL;
public:
	RESULT Init(HWND hWnd);
	void Draw(Point pos, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha = 255);

	Input* GetInput() { return input; }

	void Init(LPCWSTR gameFile);
	Scene* GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int screen_width;
	int screen_height;

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st, 
		float sr, 
		float sb,
		float &t, 
		float &nx, 
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(Point pos) { cameraPosition = pos; }

	static Game * GetInstance();

	~Game();
	Game();

	void Render();
};


