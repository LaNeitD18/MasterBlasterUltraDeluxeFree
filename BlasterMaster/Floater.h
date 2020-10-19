#pragma once
#include "GameObject.h"

#define FLOATER_WALKING_SPEED 0.00f;

#define FLOATER_BBOX_WIDTH 16
#define FLOATER_BBOX_HEIGHT 26
#define FLOATER_BBOX_HEIGHT_DIE 16

#define FLOATER_STATE_WALKING 100
#define FLOATER_STATE_DIE 200

#define FLOATER_ANI_FLY 0
#define FLOATER_ANI_DIE 2

class Floater : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	Floater();
	Floater(float x, float y);
	virtual void SetState(int state);
};

