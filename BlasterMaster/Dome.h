#pragma once
#include "GameObject.h"

#define DOME_WALKING_SPEED 0.00f;

#define DOME_BBOX_WIDTH 16
#define DOME_BBOX_HEIGHT 26
#define DOME_BBOX_HEIGHT_DIE 16

#define DOME_STATE_WALKING 100
#define DOME_STATE_DIE 200

#define DOME_ANI_TELEPORT 0
#define DOME_ANI_DIE 2

class Dome : public AnimatedGameObject
{
	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	Dome();
	Dome(float x, float y);
	virtual void SetState(int state);
};

