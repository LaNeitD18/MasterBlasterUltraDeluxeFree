#include "Sophia.h"

void Sophia::GetBoundingBox(BoundingBox & bbox)
{
	bbox.l = bbox.r = bbox.t = bbox.b = 0;
	// TODO
}

void Sophia::Update()
{
	int prevState = state;
	if (state == SOPHIA_STATE_LEFT_VEHICLE)
		return;
	Input& input = *GameGlobal::GetInput();
	bool stateAvailable = true;

	int flags = 0;
	
	bool lookedUp;
	if (state & SOPHIA_STATE_LOOKED_UP)
		lookedUp = input[VK_UP] & KEY_STATE_DOWN;
	else lookedUp = false;

	flags |= (lookedUp ? SOPHIA_STATE_LOOKED_UP : 0);

	if (pos.y < 500)
		flags |= SOPHIA_STATE_AIRBORNE;

	// Landing cannot be interrupted by moving, jumping, and looking up
	// It must finish its animation
	if ((state & 0x000f) == SOPHIA_STATE_LANDING) {
		if (stateAvailable)
			SetState(SOPHIA_STATE_LANDING | flags);
		stateAvailable = false;
	}

	// Looking up can interrupt jumping and moving
	if ((input[VK_UP] & KEY_STATE_DOWN) && !lookedUp)
	{
		flags |= SOPHIA_STATE_LOOKING_UP;
	}

	// Jumping can interrupt jump boost
	if ((state & 0x000f) == SOPHIA_STATE_JUMPING) {
		if (stateAvailable)
			SetState(SOPHIA_STATE_JUMPING | flags);
		stateAvailable = false;
	}
	
	// Jump boost
	if ((state & 0x000f) == SOPHIA_STATE_JUMP_BOOST) {
		if (stateAvailable) 
		{
			if ((input['X'] & KEY_STATE_DOWN) && jumpBoostRemaining > SOPHIA_EPSILON_THRESHOLD)
			{
				SetState(SOPHIA_STATE_JUMP_BOOST | flags);
				jumpBoostRemaining *= 0.9;
				v.y = -SOPHIA_JUMP_BOOST_AMOUNT;
			}
			else
				SetState(SOPHIA_STATE_IDLE | flags);
		}
		stateAvailable = false;
	}

	// Starting a jump must be from state moving or idle
	if (!(state & SOPHIA_STATE_AIRBORNE))
		if (input['X'] & KEY_STATE_ON_DOWN)
		{
			if (stateAvailable)
				SetState(SOPHIA_STATE_JUMPING | flags);
			jumpBoostRemaining = SOPHIA_JUMP_BOOST_AMOUNT;
			stateAvailable = false;
		}
	
	if (input[VK_LEFT] & KEY_STATE_DOWN)
		v.x = -SOPHIA_WALKING_SPEED;
	else if (input[VK_RIGHT] & KEY_STATE_DOWN)
		v.x = SOPHIA_WALKING_SPEED;
	else
		v.x = 0;

	pos += dx();

	// psuedo gravity
	if (pos.y < 800)
		if (v.y >= SOPHIA_EPSILON_THRESHOLD)
			v.y *= 1.1;
		else if (v.y <= -SOPHIA_EPSILON_THRESHOLD)
			v.y *= 0.8;
		else v.y = SOPHIA_EPSILON_THRESHOLD;
	if (pos.y > 500 && v.y > 0) {
		pos.y = 500; v.y = 0;
	}

	if (pos.y > 495 && v.y > 0)
	{
		if (stateAvailable)
			SetState(SOPHIA_STATE_LANDING | flags);
		stateAvailable = false;
	}

	if (v.length() == 0)
	{
		if (stateAvailable)
			SetState(SOPHIA_STATE_IDLE | flags);
		stateAvailable = false;
	}
	else if (pos.y == 500 && v.y == 0)
	{
		if (stateAvailable)
			SetState(SOPHIA_STATE_WALKING | flags);
		stateAvailable = false;
	}
	
	if (prevState != state)
		SetAniByState(state);
}

void Sophia::Render()
{
	currentAnimation->Render(pos, currentTime, previousFrame);
	if (!moving)
		return;
	currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		switch (state & 0x000f)
		{
			// After landing return to idle
		case SOPHIA_STATE_LANDING:
			SetState(state & (~0x000f));
			SetAniByState(this->state);
			break;
			// after jumping go to jump boost
		case SOPHIA_STATE_JUMPING:
			SetState(SOPHIA_STATE_JUMP_BOOST | (state & (~0x000f)));
			SetAniByState(this->state);
			break;
		default:
			break;
		} 
		// after looking up go to looked up
		if (state & 0x2000)
		{
			SetState(SOPHIA_STATE_LOOKED_UP | (state & (~SOPHIA_STATE_LOOKING_UP)));
			SetAniByState(this->state);
		}
		currentTime %= currentAnimation->GetLoopDuration();
		previousFrame = 0;
	}
}

void Sophia::SetAnimationType(int ANI)
{
	Animation* trg = animationSet->at(ANI);
	if (currentAnimation != trg)
	{
		currentAnimation = trg;
		// this is definitely spaghetti code
		currentTime = currentAnimation->RewindFrameTime(previousFrame);
	}
}

Sophia::Sophia()
{
	SetState(-1);
}

Sophia::Sophia(float x, float y)
{
	SetState(-1);
	pos = Point(x, y);
}

void Sophia::SetState(int state)
{
	GameObject::SetState(state);
}

void Sophia::SetAniByState(int state)
{
	if (state & SOPHIA_STATE_LEFT_VEHICLE)
		SetAnimationType(SOPHIA_ANI_LEFT_VEHICLE);
	else if (state & SOPHIA_STATE_LOOKING_UP)
		SetAnimationType(SOPHIA_ANI_LOOKING_UP);
	else if (state & SOPHIA_STATE_LOOKED_UP)
	{
		switch (state & 0x000f)
		{
		case SOPHIA_STATE_IDLE:
			SetAnimationType(SOPHIA_ANI_LOOKED_UP_IDLE);
			break;
		case SOPHIA_STATE_WALKING:
			SetAnimationType(SOPHIA_ANI_LOOKED_UP_WALKING);
			break;
		case SOPHIA_STATE_JUMPING:
			SetAnimationType(SOPHIA_ANI_LOOKED_UP_JUMPING);
			break;
		case SOPHIA_STATE_JUMP_BOOST:
			SetAnimationType(SOPHIA_ANI_LOOKED_UP_JUMP_BOOST);
			break;
		case SOPHIA_STATE_LANDING:
			SetAnimationType(SOPHIA_ANI_LOOKED_UP_LANDING);
			break;
		default:
			break;
		}
	}
	else if (state & SOPHIA_STATE_TURNING)
		SetAnimationType(SOPHIA_ANI_TURNING);
	else
	{
		switch (state & 0x000f)
		{
		case SOPHIA_STATE_IDLE:
			SetAnimationType(SOPHIA_ANI_IDLE);
			break;
		case SOPHIA_STATE_WALKING:
			SetAnimationType(SOPHIA_ANI_WALKING);
			break;
		case SOPHIA_STATE_JUMPING:
			SetAnimationType(SOPHIA_ANI_JUMPING);
			break;
		case SOPHIA_STATE_JUMP_BOOST:
			SetAnimationType(SOPHIA_ANI_JUMP_BOOST);
			break;
		case SOPHIA_STATE_LANDING:
			SetAnimationType(SOPHIA_ANI_LANDING);
			break;
		default:
			break;
		}
	}
}
