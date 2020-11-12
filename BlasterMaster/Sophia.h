#pragma once
#include <Windows.h>
#include "Interactable.h"
#include "GameObject.h"
#include "GameGlobal.h"

#define SOPHIA_WALKING_SPEED 0.03f
#define SOPHIA_ACCELERATION 0.0005f
#define SOPHIA_JUMP_BOOST_AMOUNT 0.2f
#define SOPHIA_JUMP_POWER 0.057f
#define SOPHIA_JUMP_BOOST_DECAY 0.9f

#define SOPHIA_ACTION_AMOUNT 14
#define SOPHIA_MAX_HEALTH 80

#define SOPHIA_BBOX_OFFSET_LEFT -10
#define SOPHIA_BBOX_OFFSET_RIGHT 10
#define SOPHIA_BBOX_OFFSET_TOP 4.01f
#define SOPHIA_BBOX_OFFSET_BOTTOM 20

#define SOPHIA_EPSILON_THRESHOLD 0.01f
#define SOPHIA_FALL_ACCELERATE_COEFFICIENT 1.05f
#define SOPHIA_FALL_DECELERATE_COEFFICIENT 0.9f
#define SOPHIA_FALL_MAX_SPEED 0.1f

#define SOPHIA_WHEEL_DURATION 4

/*
Actions:
	Walking
	Jumping
*/
enum SophiaState
{
	SOPHIA_STATE_IDLE					= 0x0000,	// 
	SOPHIA_STATE_WALKING				= 0x0001,	// moving and touching ground
	SOPHIA_STATE_JUMPING				= 0x0002,	// touching ground & ON_DOWN 'X'
	SOPHIA_STATE_JUMP_BOOST				= 0x0004,	// previous jumping / jump boost & down'X'
	SOPHIA_STATE_LANDING				= 0x0008,	// about to touch ground

	SOPHIA_STATE_LEAVING_VEHICLE		= 0x0200,
	SOPHIA_STATE_ENTERING_VEHICLE		= 0x0100,
	SOPHIA_STATE_LEFT_VEHICLE			= 0x8000,

	SOPHIA_STATE_LOOKED_UP				= 0x4000, 
	SOPHIA_STATE_LOOKING_LEFT			= 0x0400,

	SOPHIA_STATE_LOOKING_UP				= 0x2000,	// this animation overrides normal ones
	SOPHIA_STATE_TURNING				= 0x1000,

	SOPHIA_STATE_AIRBORNE				= 0x0800,

	SOPHIA_STATE_DYING					= 0x0080,
	
	//SOPHIA_STATE_TAKING_DAMAGE			= 0x0010,
};

enum SophiaAni {
	SOPHIA_ANI_LEAVING_VEHICLE		= 0,
	SOPHIA_ANI_IDLE					= 1,
	SOPHIA_ANI_LEFT_VEHICLE			= 1,
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
	SOPHIA_ANI_DYING				= 13,
};
class JasonSideView;

class Sophia :
	public Player
{
	double jumpBoostRemaining;

	vector<AnimationSet*> animations;
	int currentTime[SOPHIA_ACTION_AMOUNT];
	int currentFrame[SOPHIA_ACTION_AMOUNT];
	bool currentAni[SOPHIA_ACTION_AMOUNT];
	int currentSet;
	int currentWheelTime;
	int targetTime;
	int targetFrame;
	int targetAni;
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void StartAnimationType(int ANI);
	virtual void EndAnimationType(int ANI);

	void GoLeft();
	void GoRight();
	void GoHalt();

	virtual bool IsPrimaryPlayer() override;

	Sophia();
	Sophia(float x, float y);
	virtual void SetState(int state) override;
	void SetAniByState(int state);
	virtual void SetAnimationSet(AnimationSet* set) override;
	virtual void TakeDamage(int) override;
};

