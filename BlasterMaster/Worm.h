#pragma once
#include "GameObject.h"

#define WORM_WALKING_SPEED 0.03f;

#define WORM_BBOX_WIDTH 16
#define WORM_BBOX_HEIGHT 26
#define WORM_BBOX_HEIGHT_DIE 16

#define WORM_STATE_WALKING 100
#define WORM_STATE_DIE 200

#define WORM_ANI_WALKING_LEFT 0
#define WORM_ANI_WALKING_RIGHT 1
#define WORM_ANI_DIE 2

class Worm : public Enemy
{
	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	Worm();
	Worm(float x, float y);
	virtual void SetState(int state);
};

