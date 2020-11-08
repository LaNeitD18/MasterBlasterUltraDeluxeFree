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
};

class Enemy : public AnimatedGameObject
{};

class AnimatedScene : public AnimatedGameObject
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
};
