#pragma once
#include "AnimatedGameObject.h"

#define DOME_WALKING_SPEED_X 0.01f;
#define DOME_WALKING_SPEED_Y 0.005f;

#define DOME_BBOX_WIDTH 8
#define DOME_BBOX_HEIGHT 8
#define DOME_BBOX_HEIGHT_DIE 16

#define DOME_STATE_WALKING 100
#define DOME_STATE_DIE 200

#define DOME_ANI_WALKING 0
#define DOME_ANI_DIE 2

#define ROTATE_90DEGREE_TO_RADIAN 1.5707963268

class Dome : public Enemy
{
	Point previousVelocity;
	Point direction;

public:
	Dome();
	Dome(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

	void WalkRight();
	void WalkLeft();
	void WalkUp();
	void WalkDown();
};

