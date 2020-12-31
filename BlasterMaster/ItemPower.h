#pragma once
#include "AnimatedGameObject.h"

#define ITEM_POWER_BBOX_OFFSET_RIGHT 8 -1
#define ITEM_POWER_BBOX_OFFSET_BOTTOM 8 -1
#define ITEM_POWER_BBOX_OFFSET_TOP -8 +1
#define ITEM_POWER_BBOX_OFFSET_LEFT -8 +1

#define ITEM_POWER_ANIMATION_SET_ID 201

class ItemPower : public AnimatedGameObject
{
	int time;
public:
	ItemPower();
	ItemPower(Point pos, int special);

	int special;

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

};

