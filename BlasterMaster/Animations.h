#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <unordered_map>

#include "useful_stuff.h"
#include "Sprites.h"

/*
Sprite animation
*/
class CAnimationFrame
{
	Sprite* sprite;
	DWORD time;

public:
	CAnimationFrame(Sprite* sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	Sprite* GetSprite() { return sprite; }
};

class CAnimation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	vector<CAnimationFrame*> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);

	void Render(Point pos, int alpha = 255);
};

class CAnimations
{
	static CAnimations * __instance;

	unordered_map<int, CAnimation*> animations;

public:
	void Add(int id, CAnimation* ani);
	CAnimation* Get(int id);
	void Clear();

	static CAnimations * GetInstance();
};

typedef vector<CAnimation*> CAnimationSet;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets * __instance;

	unordered_map<int, CAnimationSet*> animation_sets;

public:
	CAnimationSets();
	void Add(int id, CAnimationSet* ani);
	CAnimationSet* Get(unsigned int id);


	static CAnimationSets * GetInstance();
};