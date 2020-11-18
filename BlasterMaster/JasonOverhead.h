#pragma once
#include "GameObject.h"

#define JASONO_WALKING_SPEED 0.02f
#define JASONO_ACCELERATION 0.001f
#define JASONO_MAX_HEALTH 100

#define JASONO_BBOX_WIDTH  12
#define JASONO_BBOX_HEIGHT 16

enum JasonOverheadState
{
	JASONO_STATE_IDLE = 0,	// 
	JASONO_STATE_WALKING = 1,	// moving and touching ground

	JASONO_STATE_LOOKING_LEFT = 2,
	JASONO_STATE_GOING_UP = 3,
	JASONO_STATE_GOING_DOWN = 4,

	JASONO_STATE_DYING = 5,
	JASONO_STATE_DEAD = 6,
	//JASON_STATE_TAKING_DAMAGE = 0x0100,
};	

enum JasonOverheadAni {
	JASONO_ANI_GO_UP = 0,
	JASONO_ANI_WALK = 1,
	JASONO_ANI_GO_DOWN = 2,
	JASONO_ANI_DEAD = 3,
};

class JasonOverhead :
    public Player
{
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
	bool isFlipVertical = false;
	
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Render();
	virtual void Update();

	virtual void SetState(int newState);
	virtual void SetAnimationType(int ANI);

	void GoLeft();
	void GoRight();
	void GoUp();
	void GoDown();
	void GoHalt();

	JasonOverhead();
	JasonOverhead(float, float);
	virtual ~JasonOverhead();
};

