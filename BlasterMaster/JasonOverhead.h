#pragma once
#include "Player.h"
#include "Bullet.h"

#define JASONO_WALKING_SPEED 0.6f
#define JASONO_ACCELERATION 0.033f
#define JASONO_MAX_HEALTH 80

#define JASONO_BBOX_OFFSET_LEFT		-10
#define JASONO_BBOX_OFFSET_RIGHT	10
#define JASONO_BBOX_OFFSET_TOP		-14
#define JASONO_BBOX_OFFSET_BOTTOM	15

#define JASONO_MAX_BULLET_POWER 80
#define JASONO_SINE_BULLET_POWER_THRESHOLD 60
#define JASONO_POWER_MIN_STEP 10

#define JASONO_BULLET_SPEED 2.0f
#define JASONO_GRENADE_SPEED 3.0f

enum JasonOverheadState
{
	JASONO_STATE_IDLE = 0,	// 

	JASONO_STATE_LOOKING_RIGHT = 1,	
	JASONO_STATE_LOOKING_LEFT = 0x02,
	JASONO_STATE_LOOKING_UP = 0x04,
	JASONO_STATE_LOOKING_DOWN = 0x08,

	JASONO_STATE_DYING = 0x10,
	JASONO_STATE_DEAD = 0x20,
	//JASON_STATE_TAKING_DAMAGE = 0x0100,
};	

enum JasonOverheadAni {
	JASONO_ANI_GO_UP = 0,
	JASONO_ANI_WALK = 1,
	JASONO_ANI_GO_DOWN = 2,
	JASONO_ANI_DYING = 3,
	JASONO_ANI_DEAD = 4,
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
	void GoHaltHorizontal();
	void GoHaltVertical();
	void ShootNorm();
	void ShootGrenade();

	JasonOverhead();
	JasonOverhead(float, float);
	virtual ~JasonOverhead();

	int bulletPower;
	virtual void TakeDamage(int damage);
};

