#pragma once
#include "GameObject.h"
#include "Bullet.h"
#include <unordered_set>

#define JASONO_WALKING_SPEED 0.66f
#define JASONO_ACCELERATION 0.033f
#define JASONO_MAX_HEALTH 100

#define JASONO_BBOX_WIDTH  12
#define JASONO_BBOX_HEIGHT 16
#define JASONO_BBOX_OFFSET_LEFT		-8
#define JASONO_BBOX_OFFSET_RIGHT	8
#define JASONO_BBOX_OFFSET_TOP		-1
#define JASONO_BBOX_OFFSET_BOTTOM	15

#define JASONO_MAX_BULLET_POWER 80
#define JASONO_SINE_BULLET_POWER_THRESHOLD 60
#define JASONO_POWER_MIN_STEP 10

enum JasonOverheadState
{
	JASONO_STATE_IDLE = 0,	// 
	JASONO_STATE_WALKING = 1,	// moving and touching ground

	JASONO_STATE_LOOKING_LEFT = 0x02,
	JASONO_STATE_GOING_UP = 0x04,
	JASONO_STATE_GOING_DOWN = 0x08,

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
    public Player, Manager<Bullet>
{
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
	bool isFlipVertical = false;
	unordered_set<Bullet*> bullets;
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
	void ShootNorm();
	void ShootGrenade();

	JasonOverhead();
	JasonOverhead(float, float);
	virtual ~JasonOverhead();

	int bulletPower;
	virtual void TakeDamage(int damage);

	// Inherited via Manager
	virtual void AddElement(Bullet *) override;
	virtual void RemoveElement(Bullet *) override;
};

