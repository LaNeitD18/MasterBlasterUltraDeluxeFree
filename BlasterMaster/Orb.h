#pragma once
#include "AnimatedGameObject.h"

#define ORB_BBOX_OFFSET_LEFT	  (-9 + 1)
#define ORB_BBOX_OFFSET_RIGHT	  (9 - 1)
#define ORB_BBOX_OFFSET_TOP       (-9 + 1)
#define ORB_BBOX_OFFSET_BOTTOM    (9 - 1)

enum OrbState : int {
	ORB_STATE_FLYING		= 100,
	ORB_STATE_EXPLODE		= 101
};

enum OrbAni : int {
	ORB_ANI_FLY		= 0,
	ORB_ANI_EXPLODE	= 1
};

class Orb : public Enemy
{
	int initialTime;
public:
	Orb();
	Orb(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

