#include "GameGlobal.h"

HINSTANCE GameGlobal::mHInstance = NULL;
HWND GameGlobal::mHwnd = NULL;
LPD3DXSPRITE GameGlobal::mSpriteHandler = NULL;
int GameGlobal::mWidth = 270;
int GameGlobal::mHeight = 278;
int GameGlobal::mScreenWidth = 950;
int GameGlobal::mScreenHeight = 570;
LPDIRECT3DDEVICE9 GameGlobal::mDevice = nullptr;
bool GameGlobal::isGameRunning = true;
IDirect3DSurface9* GameGlobal::backSurface = nullptr;
Input* GameGlobal::input = NULL;

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

int GameGlobal::GetScreenWidth()
{
	return mScreenWidth;
}

int GameGlobal::GetScreenHeight()
{
	return mScreenHeight;
}

void GameGlobal::SetHeight(int height)
{
	mHeight = height;
}

int GameGlobal::GetHeight()
{
	return mHeight;
}