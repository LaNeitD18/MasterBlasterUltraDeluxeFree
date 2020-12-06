#pragma once
#include "AnimatedGameObject.h"

#define SKULL_BBOX_OFFSET_LEFT	  (-9 + 1)
#define SKULL_BBOX_OFFSET_RIGHT	  (9 - 1)
#define SKULL_BBOX_OFFSET_TOP     (-10.5 + 1.5)
#define SKULL_BBOX_OFFSET_BOTTOM  (10.5 - 1)

#define SKULL_SPEED_FLYING_X		1.0f
#define SKULL_SPEED_FLYING_Y		1.0f

enum SkullState : int {
	SKULL_STATE_INIT		= 99,
	SKULL_STATE_FLYING_X	= 100,
	SKULL_STATE_FLYING_Y	= 101,
	SKULL_STATE_BOMBING		= 102
};

enum SkullAni : int {
	SKULL_ANI_FLY			= 0,
	SKULL_ANI_BOMB			= 1
};

class Skull : public Enemy
{
	int timeToChangeState;
	int previousState;
	void FlyVertically(Point playerPos);
	void FlyHorizontally(Point playerPos);
	void Bombing(Point playerPos);

public:
	Skull();
	Skull(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

