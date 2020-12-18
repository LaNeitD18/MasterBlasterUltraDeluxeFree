#pragma once
#include "AnimatedGameObject.h"

#define BODY_BBOX_OFFSET_LEFT	   -60 + 1
#define BODY_BBOX_OFFSET_RIGHT		60 - 1
#define BODY_BBOX_OFFSET_TOP	   -66 + 1
#define BODY_BBOX_OFFSET_BOTTOM		66 - 1

class Boss : public Enemy
{
public:
	Boss();
	Boss(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

#define BOSS_ARM_TIME_TO_REACH_TARGET_LOCATION 2
class BossArm : public Enemy
{
	BossArm();
	BossArm(float x, float y);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();

	virtual void Update();
	Point targetLocation;
};
