#pragma once
#include "AnimatedGameObject.h"

#define ITEM_GUN_BBOX_OFFSET_RIGHT 8 -1
#define ITEM_GUN_BBOX_OFFSET_BOTTOM 8 -1
#define ITEM_GUN_BBOX_OFFSET_TOP -8 +1
#define ITEM_GUN_BBOX_OFFSET_LEFT -8 +1

#define ITEM_GUN_ANIMATION_SET_ID 203

#define ITEM_HOMING_ANIMATION_SET_ID 204
#define ITEM_MULTI_ANIMATION_SET_ID 205
#define ITEM_THUNDER_ANIMATION_SET_ID 206

class ItemGun : public AnimatedGameObject
{
	int time;
public:
	ItemGun();
	ItemGun(Point pos);
	ItemGun(Point pos, int t);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	int type; // 1 - homing 2 - thunder 3 - multi 4 - special 0 - gun_norm

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

};

