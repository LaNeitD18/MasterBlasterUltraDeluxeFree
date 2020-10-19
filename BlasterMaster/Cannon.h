#pragma once
#include "GameObject.h"

#define CANNON_WALKING_SPEED 0.00f;

#define CANNON_BBOX_WIDTH 16
#define CANNON_BBOX_HEIGHT 26
#define CANNON_BBOX_HEIGHT_DIE 16

#define CANNON_STATE_WALKING 100
#define CANNON_STATE_DIE 200

#define CANNON_ANI_TELEPORT 0
#define CANNON_ANI_DIE 2

class Cannon : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	Cannon();
	Cannon(float x, float y);
	virtual void SetState(int state);
};

