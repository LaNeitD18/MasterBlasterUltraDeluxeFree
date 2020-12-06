#pragma once
#include "AnimatedGameObject.h"

#define CANNON_BULLET_BBOX_OFFSET_RIGHT 4 -1
#define CANNON_BULLET_BBOX_OFFSET_BOTTOM 4 -1
#define CANNON_BULLET_BBOX_OFFSET_TOP -4 +1
#define CANNON_BULLET_BBOX_OFFSET_LEFT -4 +1

#define CANNON_BULLET_ANIMATION_SET_ID 101

enum CannonBulletState : int {
	CANNON_BULLET_STATE_NORMAL = 100,
	CANNON_BULLET_STATE_EXPLODE = 101
};

enum CannonBulletAni : int {
	CANNON_BULLET_ANI_NORMAL = 0,
	CANNON_BULLET_ANI_EXPLODE = 1
};

class CannonBullet : public AnimatedGameObject
{
public:
	CannonBullet();
	CannonBullet(Point pos, Point playerPos);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

