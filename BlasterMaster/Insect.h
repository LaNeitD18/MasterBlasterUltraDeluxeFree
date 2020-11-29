#pragma once
#include "AnimatedGameObject.h"

#define INSECT_FLYING_SPEED_X		 0.15f;
#define INSECT_FLYING_SPEED_Y_UP	-0.8f;
#define INSECT_FLYING_SPEED_Y_DOWN	 0.15f;

#define INSECT_BBOX_WIDTH 9
#define INSECT_BBOX_HEIGHT 9
#define INSECT_BBOX_HEIGHT_DIE 16

#define INSECT_BBOX_OFFSET_LEFT	   -9 + 1
#define INSECT_BBOX_OFFSET_RIGHT	9 - 1
#define INSECT_BBOX_OFFSET_TOP	   -9 + 1
#define INSECT_BBOX_OFFSET_BOTTOM	9 - 1

#define INSECT_STATE_FLYING_UP		100
#define INSECT_STATE_FLYING_DOWN	101
#define INSECT_STATE_FALLING		102
#define INSECT_STATE_DIE			200

#define INSECT_ANI_FLYING 0
#define INSECT_ANI_DIE 2

class Insect : public Enemy
{
	float flyUpRange;
	float flyDownRange;

	void FlyUp();
	void FlyDown();
	void Fall();

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