#pragma once
#include "AnimatedGameObject.h"

#define DOME_WALKING_SPEED_X 0.33f;
#define DOME_WALKING_SPEED_Y 0.33f;

#define DOME_BBOX_WIDTH 9
#define DOME_BBOX_HEIGHT 9
#define DOME_BBOX_HEIGHT_DIE 16

#define DOME_BBOX_OFFSET_LEFT	   -9 + 2
#define DOME_BBOX_OFFSET_RIGHT		9 - 1
#define DOME_BBOX_OFFSET_TOP	   -9 + 1
#define DOME_BBOX_OFFSET_BOTTOM		9 - 1

#define DOME_STATE_WALKING_LEFT			100
#define DOME_STATE_WALKING_RIGHT		101
#define DOME_STATE_WALKING_UP			102
#define DOME_STATE_WALKING_DOWN			103
#define DOME_STATE_JUMPING_HORIZONTAL	104
#define DOME_STATE_JUMPING_VERTICAL		105
#define DOME_STATE_DIE 200

#define DOME_ANI_WALKING 0
#define DOME_ANI_DIE 2

#define ROTATE_90DEGREE_TO_RADIAN	atan(1) * 2

class Dome : public Enemy
{

public:
	Dome();
	Dome(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

	void WalkRight();
	void WalkLeft();
	void WalkUp();
	void WalkDown();

	void JumpHorizontally();
	void JumpVertically();

	void SetStateByDirection();
};

