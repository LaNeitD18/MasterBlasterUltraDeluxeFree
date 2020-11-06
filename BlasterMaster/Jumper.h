#pragma once
#include "AnimatedGameObject.h"

#define JUMPER_WALKING_SPEED 0.03f;

#define JUMPER_BBOX_WIDTH 16
#define JUMPER_BBOX_HEIGHT 26
#define JUMPER_BBOX_HEIGHT_DIE 16

#define JUMPER_STATE_WALKING 100
#define JUMPER_STATE_DIE 200

#define JUMPER_ANI_WALKING_LEFT 1
#define JUMPER_ANI_WALKING_RIGHT 0
#define JUMPER_ANI_DIE 2

class Jumper : public Enemy
{
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
	
public:
	Jumper();
	Jumper(float x, float y);
	virtual void SetState(int state);
};

