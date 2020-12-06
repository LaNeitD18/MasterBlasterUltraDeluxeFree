#pragma once
#include "AnimatedGameObject.h"

#define CANNON_BBOX_OFFSET_LEFT		(-13 + 1)
#define CANNON_BBOX_OFFSET_RIGHT	 (13 - 1)
#define CANNON_BBOX_OFFSET_TOP		(-13 + 1)
#define CANNON_BBOX_OFFSET_BOTTOM	(13 - 1)

enum CannonState : int {
	CANNON_STATE_VERTICAL	= 100,
	CANNON_STATE_HORIZONTAL = 101
};

enum CannonAni : int {
	CANNON_ANI_VERTICAL		= 0,
	CANNON_ANI_HORIZONTAL	= 1
};

class Cannon : public Enemy
{
	bool isVertical;
	void ShootHorizontally();
	void ShootVertically();
public:
	Cannon();
	Cannon(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

