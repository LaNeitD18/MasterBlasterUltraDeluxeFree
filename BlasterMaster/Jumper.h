#pragma once
#include "GameObject.h"

#define JUMPER_WALKING_SPEED 0.03f;

#define JUMPER_BBOX_WIDTH 16
#define JUMPER_BBOX_HEIGHT 26
#define JUMPER_BBOX_HEIGHT_DIE 16

#define JUMPER_STATE_WALKING 100
#define JUMPER_STATE_DIE 200

#define JUMPER_ANI_WALKING_LEFT 0
#define JUMPER_ANI_WALKING_RIGHT 1
#define JUMPER_ANI_DIE 2

class Jumper : public Enemy
{
	virtual void GetBoundingBox(BoundingBox&);
	virtual void Update();
	virtual void Render();

public:
	Jumper();
	Jumper(float x, float y);
	virtual void SetState(int state);
};

