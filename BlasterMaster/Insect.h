#pragma once
#include "AnimatedGameObject.h"

#define INSECT_WALKING_SPEED 1.0f;

#define INSECT_BBOX_WIDTH 9
#define INSECT_BBOX_HEIGHT 9
#define INSECT_BBOX_HEIGHT_DIE 16

#define INSECT_STATE_WALKING 100
#define INSECT_STATE_DIE 200

#define INSECT_ANI_WALKING 0
#define INSECT_ANI_DIE 2

class Insect : public AnimatedGameObject
{

public:
	Insect();
	Insect(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};