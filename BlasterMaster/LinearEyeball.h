#pragma once
#include "AnimatedGameObject.h"

#define EYEBALL_BBOX_OFFSET_LEFT	(-8 + 1)
#define EYEBALL_BBOX_OFFSET_RIGHT	(8 - 1)
#define EYEBALL_BBOX_OFFSET_TOP		(-8 + 1)
#define EYEBALL_BBOX_OFFSET_BOTTOM	(8 - 1)

#define EYEBALL_SPEED_FLYING_X		0.5f
#define EYEBALL_SPEED_FLYING_Y		0.5f

class LinearEyeball : public Enemy
{
	void RandomDirection();
public:
	LinearEyeball();
	LinearEyeball(Point pos, Point direction);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

