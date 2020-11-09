#pragma once
#include "AnimatedGameObject.h"

#define FLOATER_FLYING_SPEED_X 0.005f;
#define FLOATER_FLYING_SPEED_Y 0.02f;

#define FLOATER_BBOX_WIDTH 9
#define FLOATER_BBOX_HEIGHT 8
#define FLOATER_BBOX_HEIGHT_DIE 16

#define FLOATER_STATE_FLYING 100
#define FLOATER_STATE_DIE 200

#define FLOATER_ANI_FLY 0
#define FLOATER_ANI_DIE 2

class Floater : public Enemy
{

public:
	Floater();
	Floater(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

