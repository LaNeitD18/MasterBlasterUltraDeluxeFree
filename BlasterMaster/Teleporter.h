#pragma once
#include "AnimatedGameObject.h"

#define TELEPORTER_WALKING_SPEED 0.00f;

#define TELEPORTER_BBOX_WIDTH 16
#define TELEPORTER_BBOX_HEIGHT 26
#define TELEPORTER_BBOX_HEIGHT_DIE 16

#define TELEPORTER_STATE_WALKING 100
#define TELEPORTER_STATE_DIE 200

#define TELEPORTER_ANI_TELEPORT 0
#define TELEPORTER_ANI_DIE 2

class Teleporter : public Enemy
{
	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	Teleporter();
	Teleporter(float x, float y);
	virtual void SetState(int state);
};

