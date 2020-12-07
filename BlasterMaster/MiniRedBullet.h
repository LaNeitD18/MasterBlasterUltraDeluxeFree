#pragma once
#include "AnimatedGameObject.h"

#define BULLET_BBOX_OFFSET_RIGHT 3
#define BULLET_BBOX_OFFSET_BOTTOM 2.99
#define BULLET_BBOX_OFFSET_TOP -2.99
#define BULLET_BBOX_OFFSET_LEFT -3

enum BulletState : int {
	MINI_BULLET_STATE_NORMAL = 100,
	MINI_BULLET_STATE_EXPLODE = 101
};

enum MiniBulletAni : int {
	MINI_BULLET_ANI_NORMAL  = 0,
	MINI_BULLET_ANI_EXPLODE = 1
};

class MiniRedBullet : public AnimatedGameObject
{
public:
	MiniRedBullet();
	MiniRedBullet(Point pos, Point playerPos);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

