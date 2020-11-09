#pragma once
#include "GameObject.h"
class AnimatedGameObject : public GameObject
{
protected:
	//Long
	//int currentFrame;
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
	bool isFlipVertical = false;
public:
	virtual void Render();
	virtual void SetAnimationType(int ANI);

	AnimatedGameObject() {}
	virtual ~AnimatedGameObject() {}
};

class Enemy : public AnimatedGameObject
{
public:
	Enemy() {}
	virtual ~Enemy() {}
};
