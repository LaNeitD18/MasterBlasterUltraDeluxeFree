#pragma once
#include "GameObject.h"

#define LASER_WALKING_SPEED 0.00f;

#define LASER_BBOX_WIDTH 16
#define LASER_BBOX_HEIGHT 26
#define LASER_BBOX_HEIGHT_DIE 16

#define LASER_STATE_WALKING 100
#define LASER_STATE_DIE 200

#define LASER_ANI_TELEPORT 0
#define LASER_ANI_DIE 2

class Laser : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	Laser();
	Laser(float x, float y);
	virtual void SetState(int state);
};

