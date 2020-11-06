#pragma once
#include "AnimatedGameObject.h"

#define ORB_WALKING_SPEED 0.00f;

#define ORB_BBOX_WIDTH 16
#define ORB_BBOX_HEIGHT 26
#define ORB_BBOX_HEIGHT_DIE 16

#define ORB_STATE_WALKING 100
#define ORB_STATE_DIE 200

#define ORB_ANI_IDLE 0
#define ORB_ANI_DIE 2

class Orb : public AnimatedGameObject
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

public:
	Orb();
	Orb(float x, float y);
	virtual void SetState(int state);
};

