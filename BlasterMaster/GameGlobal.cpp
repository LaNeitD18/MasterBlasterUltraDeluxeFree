#include "GameGlobal.h"
#include "SceneArea2SideView.h"

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
int GameGlobal::SpecialBulletType = 1;
int GameGlobal::NumberBullet1 = 50;
int GameGlobal::NumberBullet2 = 50;
int GameGlobal::NumberBullet3 = 50;
Point GameGlobal::lastPositionSophia = Point(0, 0);
// temporary hp, set later for sophia jason
int GameGlobal::HealthPoint = 80;
int GameGlobal::SophiaHealthPoint = 100;
int GameGlobal::livesToPlay = 2;
Point GameGlobal::pointReturn = Point(56, 2955);
BoundingBox GameGlobal::boundingReturn = BoundingBox(0, 2814, 1038, 3094);

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

int GameGlobal::GetCurrentHealthPoint() { return HealthPoint; }

void GameGlobal::SetCurrentHealthPoint(int hp) { HealthPoint = hp; }

int GameGlobal::GetCurrentHealthPointSophia() { return SophiaHealthPoint; }

void GameGlobal::SetCurrentHealthPointSophia(int hp) { SophiaHealthPoint = hp; }

int GameGlobal::GetLivesToPlay() { return livesToPlay; }

void GameGlobal::SetLivesToPlay(int l) { livesToPlay = l; }

Point GameGlobal::GetLastPositionSophia()
{
	return lastPositionSophia;
}

void GameGlobal::SetLastPositionSophia(Point pos)
{
	lastPositionSophia = pos;
}

int GameGlobal::GetSpecialBulletType()
{
	return SpecialBulletType;
}

void GameGlobal::SetSpecialBulletType(int type)
{
	SpecialBulletType = type;
}

int GameGlobal::GetNumberSpecialBullet1()
{
	return NumberBullet1;
}

void GameGlobal::SetSpecialNumberBullet1(int n)
{
	NumberBullet1 = n;
}

int GameGlobal::GetNumberSpecialBullet2()
{
	return NumberBullet2;
}

void GameGlobal::SetSpecialNumberBullet2(int n)
{
	NumberBullet2 = n;
}

int GameGlobal::GetNumberSpecialBullet3()
{
	return NumberBullet3;
}

void GameGlobal::SetSpecialNumberBullet3(int n)
{
	NumberBullet3 = n;
}

Point GameGlobal::GetReturnPoint()
{
	return pointReturn;
}

void GameGlobal::SetReturnPoint(Point pos)
{
	pointReturn = pos;
}

BoundingBox GameGlobal::GetReturnBoundingBox()
{
	return boundingReturn;
}

void GameGlobal::SetReturnBoundingBox(BoundingBox bbox)
{
	boundingReturn = bbox;
}
