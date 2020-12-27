#pragma once
#include "AnimatedGameObject.h"

#define EYEBALL_BBOX_OFFSET_LEFT	(-8 + 1)
#define EYEBALL_BBOX_OFFSET_RIGHT	(8 - 1)
#define EYEBALL_BBOX_OFFSET_TOP		(-8 + 1)
#define EYEBALL_BBOX_OFFSET_BOTTOM	(8 - 1)

class EyeballSpawner : public Enemy
{
	int coolDown;
	void RandomDirection();
public:
	EyeballSpawner();
	EyeballSpawner(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

