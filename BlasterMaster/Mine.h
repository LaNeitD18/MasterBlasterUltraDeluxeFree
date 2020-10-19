#pragma once
#include "GameObject.h"

#define MINE_WALKING_SPEED 0.00f;

#define MINE_BBOX_WIDTH 16
#define MINE_BBOX_HEIGHT 26
#define MINE_BBOX_HEIGHT_DIE 16

#define MINE_STATE_WALKING 100
#define MINE_STATE_DIE 200

#define MINE_ANI_TELEPORT 0
#define MINE_ANI_DIE 2

class Mine : public AnimatedGameObject
{
	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	Mine();
	Mine(float x, float y);
	virtual void SetState(int state);
};

