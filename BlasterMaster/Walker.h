#pragma once
#include "GameObject.h"

#define WALKER_WALKING_SPEED 0.03f;

#define WALKER_BBOX_WIDTH 16
#define WALKER_BBOX_HEIGHT 26
#define WALKER_BBOX_HEIGHT_DIE 16

#define WALKER_STATE_WALKING 100
#define WALKER_STATE_DIE 200

#define WALKER_ANI_WALKING_LEFT 1
#define WALKER_ANI_WALKING_RIGHT 0
#define WALKER_ANI_DIE 2

class Walker : public AnimatedGameObject
{
	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	Walker();
	Walker(float x, float y);
	virtual void SetState(int state);
};

