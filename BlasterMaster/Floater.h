#pragma once
#include "AnimatedGameObject.h"

#define FLOATER_FLYING_SPEED_X 0.165f;
#define FLOATER_FLYING_SPEED_Y 0.66f;

#define FLOATER_BBOX_WIDTH 9
#define FLOATER_BBOX_HEIGHT 8

#define FLOATER_BBOX_OFFSET_LEFT	-9 + 1
#define FLOATER_BBOX_OFFSET_RIGHT	 9 - 1
#define FLOATER_BBOX_OFFSET_TOP		-8 + 1
#define FLOATER_BBOX_OFFSET_BOTTOM	 8 - 1

enum FloaterState : int {
	FLOATER_STATE_FLYING	= 100,
	FLOATER_STATE_SHOOTING	= 101
};

enum FloaterAni : int {
	FLOATER_ANI_FLY		= 0,
	FLOATER_ANI_SHOOT	= 1
};

class Floater : public Enemy
{
	int timeToShoot;
	int jumpingTurn;
	void Shoot();
public:
	Floater();
	Floater(float x, float y);
	//virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

