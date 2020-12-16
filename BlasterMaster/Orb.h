#pragma once
#include "AnimatedGameObject.h"

#define ORB_BBOX_OFFSET_LEFT	  (-9 + 1)
#define ORB_BBOX_OFFSET_RIGHT	  (9 - 1)
#define ORB_BBOX_OFFSET_TOP       (-9 + 1)
#define ORB_BBOX_OFFSET_BOTTOM    (9 - 1)

#define ORB_FLY_SPEED_X		0.5f
#define ORB_FLY_SPEED_Y		0.5f

enum OrbState : int {
	ORB_STATE_FLYING_TYPE_1		= 100,
	ORB_STATE_FLYING_TYPE_2		= 101,
	ORB_STATE_FLYING_Y			= 102,
	ORB_STATE_EXPLODE			= 103
};

enum OrbAni : int {
	ORB_ANI_FLY_1	= 0,
	ORB_ANI_FLY_2	= 1,
	ORB_ANI_EXPLODE	= 2
};

class Orb : public Enemy
{
	int initialTime; // time that orb type 1 wait to chase player
	int type; // type of orb
	int timeToFlyVertically;	// random time for orb type 2 to fly up or down
	float flyVerticallyRange;

	void UpdateOrbType1();	// type 1: chase player & explode
	void UpdateOrbType2();	// type 2: not chase
public:
	Orb();
	Orb(float x, float y, int type);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

