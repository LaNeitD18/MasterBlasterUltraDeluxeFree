#include "GameGlobal.h"

HINSTANCE GameGlobal::mHInstance = NULL;
HWND GameGlobal::mHwnd = NULL;
LPD3DXSPRITE GameGlobal::mSpriteHandler = NULL;
int GameGlobal::mWidth = 270;
int GameGlobal::mHeight = 280;
//int GameGlobal::mScreenWidth = 950;
//int GameGlobal::mScreenHeight = 570;
LPDIRECT3DDEVICE9 GameGlobal::mDevice = nullptr;
bool GameGlobal::isGameRunning = true;
IDirect3DSurface9* GameGlobal::backSurface = nullptr;
Input* GameGlobal::input = NULL;
AnimationSets* GameGlobal::mAniLib = NULL;
// temporary hp, set later for sophia jason
int GameGlobal::HealthPointSideView = 80;

GameGlobal::GameGlobal()
{

}


GameGlobal::~GameGlobal()
{
}

void GameGlobal::SetCurrentDevice(LPDIRECT3DDEVICE9 device)
{
	mDevice = device;
}

LPDIRECT3DDEVICE9 GameGlobal::GetCurrentDevice()
{
	return mDevice;
}


HINSTANCE GameGlobal::GetCurrentHINSTACE()
{
	return mHInstance;
}

HWND GameGlobal::getCurrentHWND()
{
	return mHwnd;
}

void GameGlobal::SetCurrentHINSTACE(HINSTANCE hInstance)
{
	mHInstance = hInstance;
}

void GameGlobal::SetCurrentHWND(HWND hWnd)
{
	mHwnd = hWnd;
}

void GameGlobal::SetCurrentSpriteHandler(LPD3DXSPRITE spriteHandler)
{
	mSpriteHandler = spriteHandler;
}

LPD3DXSPRITE GameGlobal::GetCurrentSpriteHandler()
{
	return mSpriteHandler;
}

void GameGlobal::SetInput(Input * input)
{
	GameGlobal::input = input;
}

Input * GameGlobal::GetInput()
{
	return GameGlobal::input;
}

void GameGlobal::SetWidth(int width)
{
	mWidth = width;
}

int GameGlobal::GetWidth()
{
	return mWidth;
}

void GameGlobal::SetHeight(int height)
{
	mHeight = height;
}

int GameGlobal::GetHeight()
{
	return mHeight;
}

AnimationSets * GameGlobal::GetAnimationSetLibrary() { return mAniLib; }

void GameGlobal::SetAnimationSetLibrary(AnimationSets * aniLib) { mAniLib = aniLib; }

int GameGlobal::GetHealthPointSideView() { return HealthPointSideView; }

void GameGlobal::SetHealthPointSideView(int hp) { HealthPointSideView = hp; }
