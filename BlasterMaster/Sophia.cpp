#include "Sophia.h"

void Sophia::GetBoundingBox(BoundingBox & bbox)
{
	bbox.l = bbox.r = bbox.t = bbox.b = 0;
	// TODO
}

void Sophia::Update()
{
	//*
	int prevState = state;
	int newState = state;
	if (state == SOPHIA_STATE_LEFT_VEHICLE)
		return;
	Input& input = *GameGlobal::GetInput();

	int flags = SOPHIA_STATE_LOOKING_LEFT & state;

	bool lookedUp;
	if (state & SOPHIA_STATE_LOOKED_UP)
	{
		lookedUp = input[VK_UP] & KEY_STATE_DOWN;
		if (lookedUp)
			newState |= SOPHIA_STATE_LOOKED_UP;
		else {
			newState &= ~SOPHIA_STATE_LOOKED_UP;
			EndAnimationType(SOPHIA_ANI_LOOKED_UP_IDLE);
		}
	}
	else lookedUp = false;

	flags |= (lookedUp ? SOPHIA_STATE_LOOKED_UP : 0);

	if (pos.y < 140) {
		flags |= SOPHIA_STATE_AIRBORNE;
		newState |= SOPHIA_STATE_AIRBORNE;
	}
	else
		newState &= ~SOPHIA_STATE_AIRBORNE;

	// Turning: maintain state
	// If sophia is looking left but key press right not left
	if ((state & SOPHIA_STATE_TURNING) ||
		((state & SOPHIA_STATE_LOOKING_LEFT)
			&& (input[VK_RIGHT] & KEY_STATE_DOWN)
			&& (!(input[VK_LEFT] & KEY_STATE_DOWN))) ||
		// If sophia is looking right but key press left not right
			(!(state & SOPHIA_STATE_LOOKING_LEFT)
				&& (input[VK_LEFT] & KEY_STATE_DOWN)
				&& (!(input[VK_RIGHT] & KEY_STATE_DOWN))))
	{
		flags |= SOPHIA_STATE_TURNING;
		newState |= SOPHIA_STATE_TURNING;
	}
	else
		newState &= ~SOPHIA_STATE_TURNING;

	// Looking up: maintain state
	if ((input[VK_UP] & KEY_STATE_DOWN) && !lookedUp)
	{
		flags |= SOPHIA_STATE_LOOKING_UP;
		newState |= SOPHIA_STATE_LOOKING_UP;
	}

	// Looking left: maintain state
	flags |= SOPHIA_STATE_LOOKING_LEFT & state;

	/* landing: maintain state
	if ((state & 0x000f) == SOPHIA_STATE_LANDING) {
		if (stateAvailable)
			SetState(SOPHIA_STATE_LANDING | flags);
		stateAvailable = false;
	}
	//*/

	/* Jumping: maintain state
	if ((state & 0x000f) == SOPHIA_STATE_JUMPING) {
		if (stateAvailable)
			SetState(SOPHIA_STATE_JUMPING | flags);
		stateAvailable = false;
	}
	//*/
	
	//*
	if (state & SOPHIA_STATE_JUMP_BOOST) {
		if ((input['X'] & KEY_STATE_DOWN) && jumpBoostRemaining > SOPHIA_EPSILON_THRESHOLD)
		{
			// maintain state
			jumpBoostRemaining *= 0.9;
			v.y = -SOPHIA_JUMP_POWER;
		}
		else
		{
			newState &= ~SOPHIA_STATE_JUMP_BOOST;
			jumpBoostRemaining = 0;
		}
	}
	//*/

	//*
	if (!(newState & SOPHIA_STATE_AIRBORNE) && 
		(input['X'] & KEY_STATE_ON_DOWN))
	{
		newState |= SOPHIA_STATE_JUMPING;
		jumpBoostRemaining = SOPHIA_JUMP_BOOST_AMOUNT;
	}
	//*/

	if ((input[VK_LEFT] & KEY_STATE_DOWN) && (input[VK_RIGHT] & KEY_STATE_DOWN))
		GoHalt();
	else if (input[VK_LEFT] & KEY_STATE_DOWN)
		GoLeft();
	else if (input[VK_RIGHT] & KEY_STATE_DOWN)
		GoRight();
	else
		GoHalt();

	pos += dx();

	// psuedo gravity
	if (pos.y < 800)
		if (v.y >= SOPHIA_EPSILON_THRESHOLD)
			v.y *= 1.05;
		else if (v.y <= -SOPHIA_EPSILON_THRESHOLD)
			v.y *= 0.9;
		else v.y = SOPHIA_EPSILON_THRESHOLD;
	if (pos.y >= 140 && v.y > 0) {
		pos.y = 140; v.y = 0;
	}

	if (pos.y > 135 && v.y > 0 && (newState & SOPHIA_STATE_AIRBORNE))
	{
		newState |= SOPHIA_STATE_LANDING;
	}

	if (pos.y == 140 && v.y == 0 && v.x != 0)
	{
		newState |= SOPHIA_STATE_WALKING;
	}
	else {
		newState &= ~SOPHIA_STATE_WALKING;
		EndAnimationType(SOPHIA_ANI_WALKING);
		EndAnimationType(SOPHIA_ANI_LOOKED_UP_WALKING);
	}
	/*
	if (input['Q'] & KEY_STATE_DOWN)
		newState |= SOPHIA_STATE_LEFT_VEHICLE;
	//*/
	//newState |= flags;
	
	if (prevState != newState)
	{
		SetState(newState);
		SetAniByState(newState);
	}
	else
		SetAniByState(newState &
		(SOPHIA_STATE_LOOKED_UP | SOPHIA_STATE_WALKING));
	//*/
	
}

void Sophia::Render()
{
	targetAni = SOPHIA_ANI_IDLE;
	if (currentAni[SOPHIA_ANI_WALKING])
		targetAni = SOPHIA_ANI_WALKING;
	if (currentAni[SOPHIA_ANI_JUMP_BOOST])
		targetAni = SOPHIA_ANI_JUMP_BOOST;
	if (currentAni[SOPHIA_ANI_LANDING])
		targetAni = SOPHIA_ANI_LANDING;
	if (currentAni[SOPHIA_ANI_JUMPING])
		targetAni = SOPHIA_ANI_JUMPING;
	int backAni = targetAni;
	if (currentAni[SOPHIA_ANI_TURNING])
		targetAni = SOPHIA_ANI_TURNING;
	if (currentAni[SOPHIA_ANI_LOOKED_UP_IDLE])
		targetAni = SOPHIA_ANI_LOOKED_UP_IDLE;
	if (currentAni[SOPHIA_ANI_LOOKED_UP_WALKING])
		targetAni = SOPHIA_ANI_LOOKED_UP_WALKING;
	if (currentAni[SOPHIA_ANI_LOOKED_UP_JUMP_BOOST])
		targetAni = SOPHIA_ANI_LOOKED_UP_JUMP_BOOST;
	if (currentAni[SOPHIA_ANI_LOOKED_UP_JUMPING])
		targetAni = SOPHIA_ANI_LOOKED_UP_JUMPING;
	if (currentAni[SOPHIA_ANI_LOOKED_UP_LANDING])
		targetAni = SOPHIA_ANI_LOOKED_UP_LANDING;
	if (currentAni[SOPHIA_ANI_LOOKING_UP])
		targetAni = SOPHIA_ANI_LOOKING_UP;
	if (currentAni[SOPHIA_ANI_LEFT_VEHICLE])
		targetAni = SOPHIA_ANI_LEFT_VEHICLE;

	int* targetFrame = &currentFrame[targetAni];
	targetTime = currentTime[targetAni];
	if (targetAni == SOPHIA_ANI_TURNING)
	{
		targetFrame = &currentFrame[backAni];
		targetTime = *targetFrame * 100 + targetTime % 100;
	}

	animations[currentSet]->at(targetAni)->
		Render(pos, targetTime, *targetFrame);

	vector<pair<int, bool> > stateToChange;
	for (int i = 0; i < SOPHIA_ACTION_AMOUNT; i++)
		if (currentAni[i])
		{
			currentTime[i]++;

			/*
			animations[currentSet]->at(i)->
				Render(pos, currentTime[i], currentFrame[i]);
			//*/

			if (currentTime[i] >= animations[currentSet]
				->at(i)->GetLoopDuration())
			{
				EndAnimationType(i);
				/*
				if (i == SOPHIA_ANI_WALKING ||
					i == SOPHIA_ANI_LOOKED_UP_WALKING ||
					i == SOPHIA_ANI_IDLE ||
					i == SOPHIA_ANI_LOOKED_UP_IDLE)
					StartAnimationType(i);
				//*/

				if (i == SOPHIA_ANI_JUMPING || i == SOPHIA_ANI_LOOKED_UP_JUMPING) {
					stateToChange.push_back(
						make_pair(SOPHIA_STATE_JUMP_BOOST, true));
					stateToChange.push_back(
						make_pair(SOPHIA_STATE_JUMPING, false));
				}
				if (i == SOPHIA_ANI_LANDING || i == SOPHIA_ANI_LOOKED_UP_LANDING)
					stateToChange.push_back(
						make_pair(SOPHIA_STATE_LANDING, false));
				if (i == SOPHIA_ANI_LOOKING_UP) {
					stateToChange.push_back(
						make_pair(SOPHIA_STATE_LOOKING_UP, false));
					stateToChange.push_back(
						make_pair(SOPHIA_STATE_LOOKED_UP, true));
				}
			}
			if (i == SOPHIA_ANI_TURNING && 
				currentTime[i] % 100 == 8)
				;// TODO: flip
			if (i == SOPHIA_ANI_TURNING && 
				currentTime[i] % 100 == 16)
			{
				EndAnimationType(i);
				stateToChange.push_back(
					make_pair(SOPHIA_STATE_LOOKING_LEFT, 
						!(state & SOPHIA_STATE_LOOKING_LEFT)));
				stateToChange.push_back(
					make_pair(SOPHIA_STATE_TURNING, false));
			}
		}


	this->targetFrame = *targetFrame;

	int newState = state;
	for (auto flag : stateToChange)
		if (flag.second)
			newState |= flag.first;
		else
			newState &= ~flag.first;
	if (newState != state) {
		SetState(newState);
		SetAniByState(newState);
	}

	/*
	int previousFrame = currentFrame;
	currentAnimation->Render(pos, currentTime, currentFrame);
	if (!moving)
		return;
	if (currentAnimation == animationSet->at(SOPHIA_ANI_TURNING) &&
		previousFrame != currentFrame) {
		// DEPEND ON SPRITE 
		SetState(state ^ SOPHIA_STATE_LOOKING_LEFT);
		SetState(state & (~SOPHIA_STATE_TURNING));
		// TODO: Flip the sprite
		currentTime -= 2;
		currentFrame--;
	}
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
		currentFrame = 0;
	}
	//*/
}

void Sophia::StartAnimationType(int ANI)
{
	if (currentAni[ANI])
		return;
	currentAni[ANI] = true;
	switch (ANI)
	{
	default:
		// this is definitely spaghetti code
		if (ANI != SOPHIA_ANI_TURNING)
			currentTime[ANI] = animations[currentSet]->at(ANI)
			->RewindFrameTime(currentFrame[ANI],
				animations[currentSet]->at(targetAni), 
				targetFrame);
		else
		{
			currentFrame[ANI] = 0;
			currentTime[ANI] = 0;
		}
	}
}

void Sophia::EndAnimationType(int ANI)
{
	currentTime[ANI] = 0;
	currentFrame[ANI] = 0;
	currentAni[ANI] = false;
	if (ANI == 2)
		int i = 0;
}

void Sophia::GoLeft()
{
	v.x -= SOPHIA_ACCELERATION;

	if (v.x < -SOPHIA_WALKING_SPEED)
		v.x = -SOPHIA_WALKING_SPEED;
}

void Sophia::GoRight()
{
	v.x += SOPHIA_ACCELERATION;

	if (v.x > SOPHIA_WALKING_SPEED)
		v.x = SOPHIA_WALKING_SPEED;
}

void Sophia::GoHalt()
{
	bool positive = v.x > 0;
	v.x = abs(v.x) - SOPHIA_ACCELERATION;
	if (v.x < 0)
		v.x = 0;
	if (!positive)
		v.x = -v.x;
}

Sophia::Sophia()
{
	SetState(0);
	for (int i = 0; i < SOPHIA_ACTION_AMOUNT; i++)
	{
		currentAni[i] = false;
		currentFrame[i] = false;
		currentTime[i] = false;
	}
	currentAni[1] = true;
	currentSet = 0;
}

Sophia::Sophia(float x, float y)
{
	SetState(0);
	pos = Point(x, y);
	for (int i = 0; i < SOPHIA_ACTION_AMOUNT; i++)
	{
		currentAni[i] = false;
		currentFrame[i] = false;
		currentTime[i] = false;
	}
	currentAni[1] = true;
	currentSet = 0;
}

void Sophia::SetState(int state)
{
	GameObject::SetState(state);
}

void Sophia::SetAniByState(int state)
{
	if (state & SOPHIA_STATE_LEFT_VEHICLE)
		StartAnimationType(SOPHIA_ANI_LEFT_VEHICLE);
	if (state & SOPHIA_STATE_LOOKING_UP)
		StartAnimationType(SOPHIA_ANI_LOOKING_UP);
	if (state & SOPHIA_STATE_LOOKED_UP)
	{
		if (state & SOPHIA_STATE_WALKING)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_WALKING);
		if (state & SOPHIA_STATE_JUMPING)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_JUMPING);
		if (state & SOPHIA_STATE_JUMP_BOOST)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_JUMP_BOOST);
		if (state & SOPHIA_STATE_LANDING)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_LANDING);
		if ((state & 0x000f) == 0)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_IDLE);
	}
	else
	{
		if (state & SOPHIA_STATE_WALKING)
			StartAnimationType(SOPHIA_ANI_WALKING);
		if (state & SOPHIA_STATE_JUMPING)
			StartAnimationType(SOPHIA_ANI_JUMPING);
		if (state & SOPHIA_STATE_JUMP_BOOST)
			StartAnimationType(SOPHIA_ANI_JUMP_BOOST);
		if (state & SOPHIA_STATE_LANDING)
			StartAnimationType(SOPHIA_ANI_LANDING);
		if ((state & 0x000f) == 0)
			StartAnimationType(SOPHIA_ANI_IDLE);
	}
	if (state & SOPHIA_STATE_TURNING)
		StartAnimationType(SOPHIA_ANI_TURNING);
}

void Sophia::SetAnimationSet(AnimationSet * set)
{
	animations.push_back(set);
}
