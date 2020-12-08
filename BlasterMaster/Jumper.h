#pragma once
#include "AnimatedGameObject.h"

#define JUMPER_WALKING_SPEED 0.33f
#define JUMPER_JUMPING_SPEED_X 0.01f
#define JUMPER_JUMPING_SPEED_Y 1.5f

#define JUMPER_BBOX_WIDTH 17
#define JUMPER_BBOX_HEIGHT 26

#define JUMPER_BBOX_OFFSET_LEFT	   -8.5 + 1
#define JUMPER_BBOX_OFFSET_RIGHT	8.5 - 1
#define JUMPER_BBOX_OFFSET_TOP	   -13 + 1
#define JUMPER_BBOX_OFFSET_BOTTOM	13 - 1

#define JUMPER_STATE_WALKING 100
#define JUMPER_STATE_JUMPING 101
#define JUMPER_STATE_FALLING 102
#define JUMPER_STATE_DIE 200

#define JUMPER_ANI_WALK 0
#define JUMPER_ANI_DIE 2

#define COUNTDOWN_TIME_TO_JUMP		100

class Jumper : public Enemy
{
	float jumpRange;
	int countDownTimeToJump;
	int countJump;

	void Jump();
	void Walk();
	void Fall();

public:
	Jumper();
	Jumper(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

