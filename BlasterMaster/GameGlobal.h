#pragma once
#ifndef __GAME_GLOBAL__
#define __GAME_GLOBAL__
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include "Input.h"
#include "Animations.h"

class GameGlobal
{
public:
	GameGlobal();
	~GameGlobal();

	static HINSTANCE GetCurrentHINSTACE();
	static HWND getCurrentHWND();

	static void SetCurrentHINSTACE(HINSTANCE hInstance);
	static void SetCurrentHWND(HWND hWnd);

	static void SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler);
	static LPD3DXSPRITE GetCurrentSpriteHandler();

	static void SetInput(Input* input);
	static Input* GetInput();

	static void SetCurrentDevice(LPDIRECT3DDEVICE9 device);
	static LPDIRECT3DDEVICE9 GetCurrentDevice();

	static void SetWidth(int width);
	static int GetWidth();

	static void SetHeight(int height);
	static int GetHeight();

	static LPDIRECT3DSURFACE9 backSurface;

	static AnimationSets* GetAnimationSetLibrary();
	static void SetAnimationSetLibrary(AnimationSets* aniLib);

	static int GetCurrentHealthPoint();
	static void SetCurrentHealthPoint(int hp);

	static int GetCurrentHealthPointSophia();
	static void SetCurrentHealthPointSophia(int hp);

	static int GetLivesToPlay();
	static void SetLivesToPlay(int l);

	static Point GetLastPositionSophia();
	static void SetLastPositionSophia(Point pos);

	static bool isGameRunning; //trang thai xem game co dang chay khong

private:
	static HINSTANCE mHInstance; //hInstance của windows hiện tại
	static HWND mHwnd; // hwnd hiện tại
	static LPD3DXSPRITE mSpriteHandler; //spriteHanlder hiện tại
	static int mWidth, mHeight; //size window standard
	//static int mScreenWidth, mScreenHeight; //size screen
	static LPDIRECT3DDEVICE9 mDevice; //device directX hiện tại
	static Input* input;
	static AnimationSets* mAniLib;

	static Point lastPositionSophia;

	static int HealthPoint;
	static int livesToPlay;

	static int SophiaHealthPoint;
};

#endif

