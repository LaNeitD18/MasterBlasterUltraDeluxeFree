#pragma once
#include "AnimatedGameObject.h"

#define SKULL_BULLET_BBOX_OFFSET_RIGHT 5 -1
#define SKULL_BULLET_BBOX_OFFSET_BOTTOM 5 -1
#define SKULL_BULLET_BBOX_OFFSET_TOP -5 +1
#define SKULL_BULLET_BBOX_OFFSET_LEFT -5 +1

#define SKULL_BULLET_ANIMATION_SET_ID 100

enum SkullBulletState : int {
	SKULL_BULLET_STATE_NORMAL		= 100,
	SKULL_BULLET_STATE_EXPLODE		= 101
};

enum SkullBulletAni : int {
	SKULL_BULLET_ANI_NORMAL			= 0,
	SKULL_BULLET_ANI_EXPLODE		= 1
};

class SkullBullet : public AnimatedGameObject
{
	int timeToExplode;
public:
	SkullBullet();
	SkullBullet(Point pos, Point playerPos);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

