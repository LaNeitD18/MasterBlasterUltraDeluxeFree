#pragma once
#include "AnimatedGameObject.h"

#define WORM_WALKING_SPEED 0.5f;
#define WORM_FALLING_SPEED_Y 1.0f;
#define WORM_FALLING_SPEED_X 0.23f;
#define WORM_JUMPING_SPEED_Y 0.5f;

#define WORM_BBOX_WIDTH 9.5
#define WORM_BBOX_HEIGHT 5
#define WORM_BBOX_HEIGHT_DIE 16

#define WORM_BBOX_OFFSET_LEFT	-9.5 + 1
#define WORM_BBOX_OFFSET_RIGHT	 9.5 - 1
#define WORM_BBOX_OFFSET_TOP    -5 + 1
#define WORM_BBOX_OFFSET_BOTTOM  5

#define WORM_STATE_WALKING 100
#define WORM_STATE_FALLING  101
#define WORM_STATE_JUMPING  102
#define WORM_STATE_DIE 200

#define WORM_ANI_IDLING 0
#define WORM_ANI_WALKING_RIGHT 1
#define WORM_ANI_DIE 2

class Worm : public Enemy
{
	float jumpRange = 0;

	void Fall();
	void Walk();
	void Jump();

public:
	Worm();
	Worm(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

