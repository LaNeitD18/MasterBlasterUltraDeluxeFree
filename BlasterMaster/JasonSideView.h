#pragma once
#include "GameObject.h"
#include "Bullet.h"

#define JASON_GRAVITY 0.066f
#define JASON_ACCELERATION 0.033f
#define JASON_WALKING_SPEED 0.66f
#define JASON_CRAWLING_SPEED 0.166f
#define JASON_JUMP_SPEED 2.05f
#define JASON_ENTER_VEHICLE_JUMP_SPEED 1.45f
#define JASON_ENTER_VEHICLE_DISAPPEAR_SPEED 1.0f
// 0.064f

#define JASON_BBOX_OFFSET_LEFT_CRAWL -6
#define JASON_BBOX_OFFSET_RIGHT_CRAWL 6
#define JASON_BBOX_OFFSET_TOP_CRAWL 2
#define JASON_BBOX_OFFSET_LEFT_NORMAL -4
#define JASON_BBOX_OFFSET_RIGHT_NORMAL 4
#define JASON_BBOX_OFFSET_TOP_NORMAL -3
#define JASON_BBOX_OFFSET_BOTTOM 12

#define JASON_MAX_HEALTH 80
#define JASON_SIDEVIEW_ANIMATION_SET_NUMBER 16

enum JasonSideViewState
{
	JASON_STATE_IDLE			= 0x00,	// 
	JASON_STATE_WALKING			= 0x01,	// moving and touching ground

	JASON_STATE_CRAWLING		= 0x40,
	JASON_STATE_LOOKING_LEFT	= 0x04,

	JASON_STATE_CLIMBING		= 0x20,
	JASON_STATE_SWIMMING		= 0x10,

	JASON_STATE_AIRBORNE		= 0x08,
	JASON_STATE_DYING			= 0x02,
	JASON_STATE_DEAD			= 0x80,
	JASON_STATE_ENTERING_VEHICLE= 0x0200,
	//JASON_STATE_TAKING_DAMAGE = 0x0100,
};

enum JasonSideViewAni {
	JASON_ANI_STAND				= 0,
	JASON_ANI_WALK				= 1,
	JASON_ANI_CRAWL				= 2,
	JASON_ANI_CLIMB				= 3,
	JASON_ANI_SWIM				= 4,
	JASON_ANI_DYING				= 5,
	JASON_ANI_DEAD				= 6,
};
class Sophia;

class JasonSideView :
	public Player
{
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
	bool isFlipVertical = false;
public:
	// Inherited via Player
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Render();
	virtual void Update();

	virtual void SetState(int newState);
	virtual void SetAnimationType(int ANI);

	void GoLeft();
	void GoRight();
	void GoHalt();
	void Shoot();

	JasonSideView();
	JasonSideView(float, float);
	virtual ~JasonSideView();

	Sophia* sophia;
	bool isTouchingSophia;
};
