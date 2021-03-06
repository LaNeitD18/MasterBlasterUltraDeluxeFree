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
	DWORD endTime;

public:
	CAnimationFrame(Sprite* sprite, int endTime) { this->sprite = sprite; this->endTime = endTime; }
	DWORD GetEndTime() { return endTime; }
	Sprite* GetSprite() { return sprite; }
};

class Animation
{
	DWORD lastFrameTime;
	int currentFrame;
	int defaultTime;
	int loopDuration = 0;
	// Time measued in frames
	vector<CAnimationFrame*> frames;
public:
	Animation(int defaultTime = 1) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, SpriteLibrary* spriteLib, DWORD time = 0);

	void Render(Point pos, int& time, int& previousFrame, int alpha = 255);
	int GetLoopDuration() {
		return loopDuration;
	}
};

class AnimationLibrary
{
	unordered_map<int, Animation*> animations;

public:
	void Add(int id, Animation* ani);
	Animation* Get(int id);
	void Clear();
};

typedef vector<Animation*> AnimationSet;

/*
	Manage animation set database
*/
class AnimationSets
{
	static AnimationSets * __instance;

	unordered_map<int, AnimationSet*> animation_sets;

public:
	AnimationSets();
	void Add(int id, AnimationSet* ani);
	AnimationSet* Get(unsigned int id);
};