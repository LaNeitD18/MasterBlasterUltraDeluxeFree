#pragma once
#include "AnimatedGameObject.h"

#define MINE_BBOX_OFFSET_LEFT	 (-7 + 1)
#define MINE_BBOX_OFFSET_RIGHT	  (7 - 1)
#define MINE_BBOX_OFFSET_TOP     (-4 + 1)
#define MINE_BBOX_OFFSET_BOTTOM   (4 - 1)

#define MINE_ANI_NORMAL 0

class Mine : public Enemy
{
public:
	Mine();
	Mine(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

