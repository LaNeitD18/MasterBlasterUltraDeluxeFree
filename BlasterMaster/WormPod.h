#pragma once
#include "GameObject.h"

#define WORMPOD_WALKING_SPEED 0.00f;

#define WORMPOD_BBOX_WIDTH 16
#define WORMPOD_BBOX_HEIGHT 26
#define WORMPOD_BBOX_HEIGHT_DIE 16

#define WORMPOD_STATE_WALKING 100
#define WORMPOD_STATE_DIE 200

#define WORMPOD_ANI_FLY 0
#define WORMPOD_ANI_DIE 2

class WormPod : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	WormPod();
	WormPod(float x, float y);
	virtual void SetState(int state);
};

