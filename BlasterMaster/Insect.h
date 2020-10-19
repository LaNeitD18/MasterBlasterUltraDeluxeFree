#pragma once
#include "GameObject.h"

#define INSECT_WALKING_SPEED 0.03f;

#define INSECT_BBOX_WIDTH 16
#define INSECT_BBOX_HEIGHT 26
#define INSECT_BBOX_HEIGHT_DIE 16

#define INSECT_STATE_WALKING 100
#define INSECT_STATE_DIE 200

#define INSECT_ANI_WALKING_LEFT 1
#define INSECT_ANI_WALKING_RIGHT 0
#define INSECT_ANI_DIE 2

class Insect : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	Insect();
	Insect(float x, float y);
	virtual void SetState(int state);
};

