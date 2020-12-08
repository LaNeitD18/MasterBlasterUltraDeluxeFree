#pragma once
#include "AnimatedGameObject.h"

#define ORB_WALKING_SPEED 0.00f;

#define ORB_BBOX_WIDTH 16
#define ORB_BBOX_HEIGHT 26
#define ORB_BBOX_HEIGHT_DIE 16

#define ORB_STATE_WALKING 100

#define ORB_ANI_IDLE 0

class Orb : public Enemy
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Orb();
	Orb(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
};

