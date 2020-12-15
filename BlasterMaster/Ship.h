#pragma once
#include "AnimatedGameObject.h"

#define SHIP_FLYING_SPEED 0.5f

#define SHIP_BBOX_OFFSET_LEFT	 (-13 + 1)
#define SHIP_BBOX_OFFSET_RIGHT	 (13 - 1)
#define SHIP_BBOX_OFFSET_TOP     (-9 + 1)
#define SHIP_BBOX_OFFSET_BOTTOM  (9 - 1)

#define SHIP_STATE_FLYING	100

#define SHIP_ANI_FLY		0

class Ship : public Enemy
{
	int timeToShoot;
	int shootTurn;
	void Shoot();
public:
	Ship();
	Ship(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

