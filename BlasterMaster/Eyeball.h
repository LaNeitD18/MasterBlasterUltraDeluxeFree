#pragma once
#include "AnimatedGameObject.h"

#define EYEBALL_BBOX_OFFSET_LEFT	(-8 + 1)
#define EYEBALL_BBOX_OFFSET_RIGHT	(8 - 1)
#define EYEBALL_BBOX_OFFSET_TOP		(-8 + 1)
#define EYEBALL_BBOX_OFFSET_BOTTOM	(8 - 1)

#define EYEBALL_SPEED_FLYING_X		1.0f
#define EYEBALL_SPEED_FLYING_Y		1.0f

enum EyeballState : int {
	EYEBALL_STATE_FLYING		= 100,
	EYEBALL_STATE_START_FLYING	= 101,
	EYEBALL_STATE_STOP_FLYING	= 102,
	EYEBALL_STATE_IDLE			= 103,
};

enum EyeballAni : int {
	EYEBALL_ANI_OPEN_EYE	= 0,
	EYEBALL_ANI_CLOSE_EYE	= 1
};

class Eyeball : public Enemy
{
	int timeToStart;
	int timeToFly;

	void Fly();
	void Shoot();
public:
	Eyeball();
	Eyeball(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

