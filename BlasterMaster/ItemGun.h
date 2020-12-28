#pragma once
#include "AnimatedGameObject.h"

#define ITEM_GUN_BBOX_OFFSET_RIGHT 8 -1
#define ITEM_GUN_BBOX_OFFSET_BOTTOM 8 -1
#define ITEM_GUN_BBOX_OFFSET_TOP -8 +1
#define ITEM_GUN_BBOX_OFFSET_LEFT -8 +1

#define ITEM_GUN_ANIMATION_SET_ID 203

class ItemGun : public AnimatedGameObject
{
	int time;
public:
	ItemGun();
	ItemGun(Point pos);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

};

