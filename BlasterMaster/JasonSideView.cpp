#include "JasonSideView.h"
#include "GameGlobal.h"
#include "Sophia.h"

JasonSideView::JasonSideView()
{
	state = 0x1000;
	pos = Point();

	currentAnimation = NULL;
	HealthPoint = JASON_MAX_HEALTH;
	sophia = NULL;
}
JasonSideView::JasonSideView(float x, float y)
{
	state = 0x1000;
	pos = Point(x, y);
	
	currentAnimation = NULL;
	// set looking right
	drawArguments.FlipVertical(true);
	HealthPoint = JASON_MAX_HEALTH;
	sophia = NULL;
}

JasonSideView::~JasonSideView()
{
}
/*
bool JasonSideView::IsPrimaryPlayer()
{
	return !(state & JASON_STATE_ENTERING_VEHICLE);
}
//*/

BoundingBox JasonSideView::GetBoundingBox()
{
	if (state & JASON_STATE_CRAWLING)
		return BoundingBox(
		pos.x + JASON_BBOX_OFFSET_LEFT_CRAWL,
		pos.y + JASON_BBOX_OFFSET_TOP_CRAWL,
		pos.x + JASON_BBOX_OFFSET_RIGHT_CRAWL,
		pos.y + JASON_BBOX_OFFSET_BOTTOM);
	else
	return BoundingBox(
		pos.x + JASON_BBOX_OFFSET_LEFT_NORMAL,
		pos.y + JASON_BBOX_OFFSET_TOP_NORMAL,
		pos.x + JASON_BBOX_OFFSET_RIGHT_NORMAL,
		pos.y + JASON_BBOX_OFFSET_BOTTOM);
}

void JasonSideView::Render()
{
	if (currentAnimation == NULL)
		return;
	Point cameraOffset = Camera::GetInstance()->GetPosition();
	cameraOffset.x -= Camera::GetInstance()->GetWidth() / 2;
	cameraOffset.y -= Camera::GetInstance()->GetHeight() / 2;
	drawArguments.SetPosition(pos - cameraOffset);

	drawArguments.FlipVertical(isFlipVertical);

	currentAnimation->Render(currentTime, previousFrame, drawArguments);
	if (!moving)
		return;
	if (currentAnimation == animationSet->at(JASON_ANI_WALK) ||
		currentAnimation == animationSet->at(JASON_ANI_CRAWL))
	{
		if (state & JASON_STATE_WALKING)
			currentTime++;
	}
	else
		currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		if (currentAnimation == animationSet->at(JASON_ANI_DYING))
			SetAnimationType(JASON_ANI_DEAD);
		else if (currentAnimation == animationSet->at(JASON_ANI_DEAD))
			manager->RemoveElement(this);
		else
		{
			currentTime %= currentAnimation->GetLoopDuration();
			previousFrame = 0;
		}
	}
}

void JasonSideView::Update()
{
	int newState = state;
	int flags = 0;
	bool dead = (state & JASON_STATE_DYING) ||
		(state & JASON_STATE_DEAD);
	Input& input = *GameGlobal::GetInput();

	if (!wallBot) {
		flags |= JASON_STATE_AIRBORNE;
		newState |= JASON_STATE_AIRBORNE;
		v.y += JASON_GRAVITY;
	}
	else {
		newState &= ~JASON_STATE_AIRBORNE;
		if (v.y > 0)
			v.y = 0;
	}

	if (wallTop && (v.y < 0))
		v.y = 0;

	if (!(newState & JASON_STATE_AIRBORNE) &&
		(input[INPUT_DOWN] == KEY_STATE_ON_DOWN))
	{
		newState |= JASON_STATE_CRAWLING;
	}
	else
		if (!(newState & JASON_STATE_AIRBORNE) &&
			(input[INPUT_UP] == KEY_STATE_ON_DOWN))
		{
			newState &= ~JASON_STATE_CRAWLING;
		}

	if (!dead) {
		if ((input[INPUT_LEFT] & KEY_STATE_DOWN) &&
			(input[INPUT_RIGHT] & KEY_STATE_DOWN))
			GoHalt();
		else if (input[INPUT_LEFT] & KEY_STATE_DOWN) {
			GoLeft();
			newState |= JASON_STATE_LOOKING_LEFT;
			isFlipVertical = false;
		}
		else if (input[INPUT_RIGHT] & KEY_STATE_DOWN) {
			GoRight();
			newState &= ~JASON_STATE_LOOKING_LEFT;
			isFlipVertical = true;
		}
		else if (!(newState & JASON_STATE_AIRBORNE))
				GoHalt();
	}

	if (!(newState & JASON_STATE_AIRBORNE) &&
		(input[INPUT_JUMP] == KEY_STATE_ON_DOWN) &&
		!(newState & JASON_STATE_CRAWLING) &&
		!dead)
	{
		v.y = -JASON_JUMP_SPEED;
	}

	if (wallBot && v.y == 0 && v.x != 0)
	{
		newState |= JASON_STATE_WALKING;
	}
	else {
		newState &= ~JASON_STATE_WALKING;
	}

	if (v.x < 0)
		newState |= JASON_STATE_LOOKING_LEFT;

	if (v.x > 0)
		newState &= ~JASON_STATE_LOOKING_LEFT;

	if (HealthPoint <= 0)
	{
		newState |= JASON_STATE_DYING;
	}

	if (sophia != NULL &&
		input[INPUT_LEAVE_VEHICLE] == KEY_STATE_ON_DOWN)
	{
		newState |= JASON_STATE_ENTERING_VEHICLE;
		newState &= ~JASON_STATE_CRAWLING;
		newState |= JASON_STATE_AIRBORNE;
		pos.x = sophia->pos.x;
		v.x = 0;
		v.y = -JASON_ENTER_VEHICLE_JUMP_SPEED;
		sophia->SetState(sophia->GetState() | SOPHIA_STATE_ENTERING_VEHICLE);
		sophia->SetAniByState(sophia->GetState());
	}

	if ((state & JASON_STATE_ENTERING_VEHICLE) &&
		(v.y > JASON_ENTER_VEHICLE_DISAPPEAR_SPEED))
		manager->RemoveElement(this);

	Player::Update();

	wallBot = wallLeft = wallRight = wallTop = false;

	sophia = NULL;

	if (dead)
		return;

	pos += dx();
	if (newState != state)
		SetState(newState);
}

void JasonSideView::SetState(int newState)
{
	int newAni;

	if (newState & JASON_STATE_CRAWLING)
		newAni = JASON_ANI_CRAWL;
	else if (newState & JASON_STATE_WALKING)
		newAni = JASON_ANI_WALK;
	else newAni = JASON_ANI_STAND;

	if (newState & JASON_STATE_SWIMMING)
		newAni = JASON_ANI_SWIM;

	if (newState & JASON_STATE_CLIMBING)
		newAni = JASON_ANI_CLIMB;

	// Top priority is lower
	if (newState & JASON_STATE_DYING)
		newAni = JASON_ANI_DYING;

	if (currentAnimation != animationSet->at(newAni))
		SetAnimationType(newAni);

	Player::SetState(newState);
}

void JasonSideView::SetAnimationType(int ANI)
{
	Animation* trg = animationSet->at(ANI);
	if (currentAnimation != trg)
	{
		currentAnimation = trg;
		previousFrame = 0;
		currentTime = 0;
	}
}


void JasonSideView::GoLeft()
{
	v.x -= JASON_ACCELERATION;

	if (v.x < -JASON_WALKING_SPEED)
		v.x = -JASON_WALKING_SPEED;

	if (v.x < -JASON_CRAWLING_SPEED &&
		(state & JASON_STATE_CRAWLING))
		v.x = -JASON_CRAWLING_SPEED;

	if (wallLeft)
		v.x = 0;
}

void JasonSideView::GoRight()
{
	v.x += JASON_ACCELERATION;

	if (v.x > JASON_WALKING_SPEED)
		v.x = JASON_WALKING_SPEED;

	if (v.x > JASON_CRAWLING_SPEED &&
		(state & JASON_STATE_CRAWLING))
		v.x = JASON_CRAWLING_SPEED;

	if (wallRight)
		v.x = 0;
}

void JasonSideView::GoHalt()
{
	bool positive = v.x > 0;
	v.x = abs(v.x) - JASON_ACCELERATION;
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
#define CURRENT_CLASS JasonSideView
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS