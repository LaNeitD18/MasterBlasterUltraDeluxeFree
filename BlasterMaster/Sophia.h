#pragma once
#include "Interactable.h"
#include "GameObject.h"
#include "GameGlobal.h"

#define SOPHIA_WALKING_SPEED 0.03f
#define SOPHIA_JUMP_BOOST_AMOUNT 0.05f
#define SOPHIA_EPSILON_THRESHOLD 0.01f
enum SophiaState
{
	SOPHIA_STATE_IDLE					= 0x0000,		// 
	SOPHIA_STATE_WALKING				= 0x0001,		// moving and touching ground
	SOPHIA_STATE_JUMPING				= 0x0002,		// touching ground & ON_DOWN 'X'
	SOPHIA_STATE_JUMP_BOOST				= 0x0003,		// previous jumping / jump boost & down'X'
	SOPHIA_STATE_LANDING				= 0x0004,

	SOPHIA_STATE_LEFT_VEHICLE			= 0x8000,
	SOPHIA_STATE_LOOKED_UP				= 0x4000, 

	SOPHIA_STATE_LOOKING_UP				= 0x2000,
	SOPHIA_STATE_TURNING				= 0x1000,

	SOPHIA_STATE_AIRBORNE				= 0x0800,
	
	SOPHIA_STATE_TAKING_DAMAGE			= 0x0010,
};

enum SophiaAni {
	SOPHIA_ANI_LEFT_VEHICLE			= 0,
	SOPHIA_ANI_IDLE					= 1,
	SOPHIA_ANI_WALKING				= 2,
	SOPHIA_ANI_JUMPING				= 3,
	SOPHIA_ANI_JUMP_BOOST			= 4, //Note turning override airborne
	SOPHIA_ANI_LANDING				= 5,
	SOPHIA_ANI_TURNING				= 6,
	SOPHIA_ANI_LOOKING_UP			= 7,
	SOPHIA_ANI_LOOKED_UP_IDLE		= 8,
	SOPHIA_ANI_LOOKED_UP_WALKING	= 9, 
	SOPHIA_ANI_LOOKED_UP_JUMPING	= 10,
	SOPHIA_ANI_LOOKED_UP_JUMP_BOOST	= 11,
	SOPHIA_ANI_LOOKED_UP_LANDING	= 12,
};


class Sophia :
	public Player, public Interactable
{
	double jumpBoostRemaining;
public:
	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF, INTERACTABLE_GROUP);

	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();
	virtual void SetAnimationType(int ANI);

	Sophia();
	Sophia(float x, float y);
	virtual void SetState(int state);
	void SetAniByState(int state);
};

