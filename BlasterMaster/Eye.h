#pragma once
#include "GameObject.h"

#define EYE_WALKING_SPEED 0.00f;

#define EYE_BBOX_WIDTH 16
#define EYE_BBOX_HEIGHT 26
#define EYE_BBOX_HEIGHT_DIE 16

#define EYE_STATE_WALKING 100
#define EYE_STATE_DIE 200

#define EYE_ANI_TELEPORT 0
#define EYE_ANI_DIE 2

class Eye : public AnimatedGameObject
{
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update();
	virtual void Render();

public:
	Eye();
	Eye(float x, float y);
	virtual void SetState(int state);
};

