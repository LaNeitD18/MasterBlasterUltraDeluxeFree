#include "JasonOverhead.h"
#include "GameGlobal.h"
#include "Utils.h"

JasonOverhead::JasonOverhead()
{
	state = 0x01;
	pos = Point();

	currentAnimation = NULL;
	HealthPoint = JASONO_MAX_HEALTH;
}
JasonOverhead::JasonOverhead(float x, float y)
{
	pos = Point(x, y);

	SetState(JASONO_STATE_GOING_UP);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	//SetAnimationType(0);
	// set looking right
	//drawArguments.FlipVertical(true);
	HealthPoint = JASONO_MAX_HEALTH;
}

JasonOverhead::~JasonOverhead()
{
}

BoundingBox JasonOverhead::GetBoundingBox()
{
	return BoundingBox(
		pos.x - JASONO_BBOX_WIDTH,
		pos.y - JASONO_BBOX_HEIGHT + 2,
		pos.x + JASONO_BBOX_WIDTH,
		pos.y + JASONO_BBOX_HEIGHT);
}

void JasonOverhead::Render()
{
	if (state == JASONO_STATE_GOING_UP)
		SetAnimationType(JASONO_ANI_GO_UP);
	else if (state == JASONO_STATE_GOING_DOWN)
		SetAnimationType(JASONO_ANI_GO_DOWN);
	else if (state == JASONO_STATE_DEAD || state == JASONO_STATE_DYING)
		SetAnimationType(JASONO_ANI_DEAD);
	else
		SetAnimationType(JASONO_ANI_WALK);

	if (currentAnimation == NULL)
		return;
	Point cameraOffset = Camera::GetInstance()->GetPosition();
	cameraOffset.x -= Camera::GetInstance()->GetWidth() / 2;
	cameraOffset.y -= Camera::GetInstance()->GetHeight() / 2;
	drawArguments.SetPosition(pos - cameraOffset);
	//drawArguments.SetPosition(Point(100,100));

	drawArguments.FlipVertical(isFlipVertical);

	/*currentAnimation->Render(currentTime, previousFrame, drawArguments);
	if (!moving)
		return;
	if (currentAnimation == animationSet->at(JASONO_ANI_WALK))
	{
		if (state & JASONO_STATE_WALKING)
			currentTime++;
	}
	else
		currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		if (currentAnimation == animationSet->at(JASONO_ANI_DEAD))
			manager->RemoveElement(this);
		else
		{
			currentTime %= currentAnimation->GetLoopDuration();
			previousFrame = 0;
		}
	}*/

	currentAnimation->Render(currentTime, previousFrame, drawArguments);
	if (!moving)
		return;
	if(v.x != 0 || v.y != 0)
		currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		currentTime %= currentAnimation->GetLoopDuration();
		previousFrame = 0;
	}
}

void JasonOverhead::Update()
{
	pos += dx();
	Input& input = *GameGlobal::GetInput();

	int newState = state;
	int flags = 0;
	bool dead = (state == JASONO_STATE_DYING) ||
		(state == JASONO_STATE_DEAD);

	if (wallTop && (v.y < 0))
		v.y = 0;

	if (!dead) {
		if ((input[VK_LEFT] & KEY_STATE_DOWN) &&
			(input[VK_RIGHT] & KEY_STATE_DOWN))
			GoHalt();
		else if (input[VK_LEFT] & KEY_STATE_DOWN) {
			GoLeft();
			newState = JASONO_STATE_LOOKING_LEFT;
			isFlipVertical = false;
		}
		else if (input[VK_RIGHT] & KEY_STATE_DOWN) {
			GoRight();
			newState = JASONO_STATE_LOOKING_LEFT;
			isFlipVertical = true;
		}
		else if (input[VK_UP] & KEY_STATE_DOWN) {
			GoUp();
			newState = JASONO_STATE_GOING_UP;
		}
		else if (input[VK_DOWN] & KEY_STATE_DOWN) {
			GoDown();
			newState = JASONO_STATE_GOING_DOWN;
		}
		else {
			v.x = v.y = 0;
		}
	}

	/*if (wallBot && v.y == 0 && v.x != 0)
	{
		newState |= JASONO_STATE_WALKING;
	}
	else {
		newState &= ~JASONO_STATE_WALKING;
	}

	if ((input['A'] & KEY_STATE_DOWN) &&
		!(input['D'] & KEY_STATE_DOWN))
		newState = JASONO_STATE_LOOKING_LEFT;

	if ((input['D'] & KEY_STATE_DOWN) &&
		!(input['A'] & KEY_STATE_DOWN))
		newState = JASONO_STATE_LOOKING_LEFT;*/

	/*if (HealthPoint <= 0)
	{
		newState = JASONO_STATE_DYING;
	}*/

	Player::Update();

	wallBot = wallLeft = wallRight = wallTop = false;

	if (dead) {
		DebugOut(L"Dead");
		return;
	}
		
	if (newState != state)
		SetState(newState);
}

void JasonOverhead::SetState(int newState)
{
	//int newAni;

	//if (newState & JASONO_STATE_WALKING)
	//	newAni = JASONO_ANI_WALK;
	//else if (newState & JASONO_STATE_LOOKING_LEFT)
	//	newAni = JASONO_ANI_WALK;
	//else if (newState & JASONO_STATE_GOING_UP)
	//	newAni = JASONO_ANI_GO_UP;
	//else if (newState & JASONO_STATE_GOING_DOWN)
	//	newAni = JASONO_ANI_GO_DOWN;

	//// Top priority is lower
	//if (newState & JASONO_STATE_DYING)
	//	newAni = JASONO_ANI_DEAD;

	/*if (currentAnimation != animationSet->at(newAni))
		SetAnimationType(newAni);*/

	Player::SetState(newState);
}

void JasonOverhead::SetAnimationType(int ANI)
{
	Animation* trg = animationSet->at(ANI);
	if (currentAnimation != trg)
	{
		currentAnimation = trg;
		previousFrame = 0;
		currentTime = 0;
	}
}

void JasonOverhead::GoLeft()
{
	v.y = 0;
	v.x -= JASONO_ACCELERATION;

	if (v.x < -JASONO_WALKING_SPEED)
		v.x = -JASONO_WALKING_SPEED;

	if (wallLeft)
		v.x = 0;
}

void JasonOverhead::GoRight()
{
	v.y = 0;
	v.x += JASONO_ACCELERATION;

	if (v.x > JASONO_WALKING_SPEED)
		v.x = JASONO_WALKING_SPEED;

	if (wallRight)
		v.x = 0;
}

void JasonOverhead::GoUp()
{
	v.x = 0;
	v.y -= JASONO_ACCELERATION;

	if (v.y < -JASONO_WALKING_SPEED)
		v.y = -JASONO_WALKING_SPEED;

	if (wallTop)
		v.y = 0;
}

void JasonOverhead::GoDown()
{
	v.x = 0;
	v.y += JASONO_ACCELERATION;

	if (v.y > JASONO_WALKING_SPEED)
		v.y = JASONO_WALKING_SPEED;

	if (wallBot)
		v.y = 0;
}

void JasonOverhead::GoHalt()
{
	bool positive = v.x > 0;
	v.x = abs(v.x) - JASONO_ACCELERATION;
	if (v.x < 0)
		v.x = 0;
	if (!positive)
		v.x = -v.x;

	if (v.x > 0 && wallRight)
		v.x = 0;
	if (v.x < 0 && wallRight)
		v.x = 0;
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS JasonOverhead
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS