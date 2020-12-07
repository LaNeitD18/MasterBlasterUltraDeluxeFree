#pragma once
#include "AnimatedGameObject.h"

#define MINE_BULLET_BBOX_OFFSET_RIGHT 4 -1
#define MINE_BULLET_BBOX_OFFSET_BOTTOM 4 -1
#define MINE_BULLET_BBOX_OFFSET_TOP -4 +1
#define MINE_BULLET_BBOX_OFFSET_LEFT -4 +1

#define MINE_BULLET_ANIMATION_SET_ID 102

enum MineBulletState : int {
	MINE_BULLET_STATE_NORMAL = 100,
	MINE_BULLET_STATE_EXPLODE = 101
};

enum MineBulletAni : int {
	MINE_BULLET_ANI_NORMAL = 0,
	MINE_BULLET_ANI_EXPLODE = 1
};

class MineBullet : public AnimatedGameObject
{
public:
	MineBullet();
	MineBullet(Point pos, Point v);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

