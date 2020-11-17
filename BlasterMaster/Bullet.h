#pragma once
#include "AnimatedGameObject.h"

// Note: define for shooting left
#define BULLET_OFFSET_RIGHT 3
#define BULLET_OFFSET_DOWN 2.99
#define BULLET_OFFSET_UP -2.99
#define BULLET_OFFSET_LEFT -3

#define BULLET_ANIMATION_SET_NUMBER 17

enum BulletLevel :int {
	BULLET_LEVEL1			= 1,
};

enum BulletFlag : int {
	BULLET_STATE_EXPLODE	= 0x0001
};

enum BulletAni :int {
	BULLET_ANI_NORM1		= 0,
	BULLET_ANI_NORM2		= 1,
	BULLET_ANI_EXPLODE		= 2
};

class Bullet :
	public AnimatedGameObject
{
protected:
	int level;
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	Bullet();
	Bullet(Point pos, Point v, int level = 1);
	virtual ~Bullet();

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();

	virtual void Render();
	virtual void SetAnimationSet(AnimationSet* aniSet);

	int GetLevel();
};

