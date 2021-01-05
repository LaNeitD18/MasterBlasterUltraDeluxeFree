#include "Sophia.h"
#include "Camera.h"
#include "JasonSideView.h"
#include "Bullet.h"
#include "Sound.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"


BoundingBox Sophia::GetBoundingBox()
{
	return BoundingBox(
		pos.x + SOPHIA_BBOX_OFFSET_LEFT,
		pos.y + SOPHIA_BBOX_OFFSET_TOP,
		pos.x + SOPHIA_BBOX_OFFSET_RIGHT,
		pos.y + SOPHIA_BBOX_OFFSET_BOTTOM);
	// TODO
}

void Sophia::Update()
{
	Player::Update();
	if (state & SOPHIA_STATE_DYING)
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));

	if ((state & SOPHIA_STATE_LEAVING_VEHICLE) ||
		(state & SOPHIA_STATE_LEFT_VEHICLE) ||
		(state & SOPHIA_STATE_DYING))
		return;
	pos += dx();

	//*
	int prevState = state;
	int newState = state;
	if ((state & SOPHIA_STATE_LEAVING_VEHICLE) &&
		(state & SOPHIA_STATE_LEFT_VEHICLE))
		return;
	Input& input = *GameGlobal::GetInput();

	int flags = SOPHIA_STATE_LOOKING_LEFT & state;


	if ((state & SOPHIA_STATE_AIRBORNE) && v.y < 0)
	{
		newState |= SOPHIA_STATE_HANGING_WHEEL;
	}
	else
	{
		newState &= ~SOPHIA_STATE_HANGING_WHEEL;
	}

	// psuedo gravity
	if (!wallBot || v.y < 0) // If not touching ground or going up
		if (v.y >= SOPHIA_EPSILON_THRESHOLD)
		{
			if (v.y < SOPHIA_FALL_MAX_SPEED)
			v.y *= SOPHIA_FALL_ACCELERATE_COEFFICIENT;
		}
		else
		{
			if (v.y <= -SOPHIA_EPSILON_THRESHOLD)
				v.y *= SOPHIA_FALL_DECELERATE_COEFFICIENT;
			else v.y = SOPHIA_EPSILON_THRESHOLD;
			if (wallTop)
				v.y = SOPHIA_EPSILON_THRESHOLD;
		}
	else {
		v.y = 0;
	}

	bool lookedUp;
	if (state & SOPHIA_STATE_LOOKED_UP)
	{
		lookedUp = input[INPUT_UP] & KEY_STATE_DOWN;
		if (lookedUp)
			newState |= SOPHIA_STATE_LOOKED_UP;
		else {
			newState &= ~SOPHIA_STATE_LOOKED_UP;
			EndAnimationType(SOPHIA_ANI_LOOKED_UP_IDLE);
		}
	}
	else lookedUp = false;

	flags |= (lookedUp ? SOPHIA_STATE_LOOKED_UP : 0);

	if (!wallBot) {
		flags |= SOPHIA_STATE_AIRBORNE;
		newState |= SOPHIA_STATE_AIRBORNE;
	}
	else {
		newState &= ~SOPHIA_STATE_AIRBORNE;
		if (state & SOPHIA_STATE_AIRBORNE)
			Sound::getInstance()->play("sophia_fall_ground", false, 1);
	}

	// Turning: maintain state
	// If sophia is looking left but key press right not left
	if ((state & SOPHIA_STATE_TURNING) ||
		((state & SOPHIA_STATE_LOOKING_LEFT)
			&& (input[INPUT_RIGHT] & KEY_STATE_DOWN)
			&& (!(input[INPUT_LEFT] & KEY_STATE_DOWN))) ||
		// If sophia is looking right but key press left not right
			(!(state & SOPHIA_STATE_LOOKING_LEFT)
				&& (input[INPUT_LEFT] & KEY_STATE_DOWN)
				&& (!(input[INPUT_RIGHT] & KEY_STATE_DOWN))))
	{
		flags |= SOPHIA_STATE_TURNING;
		newState |= SOPHIA_STATE_TURNING;
	}
	else
		newState &= ~SOPHIA_STATE_TURNING;

	// Looking up: maintain state
	if ((input[INPUT_UP] & KEY_STATE_DOWN) && !lookedUp)
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
		if ((input[INPUT_JUMP] & KEY_STATE_DOWN) &&
			jumpBoostRemaining > SOPHIA_EPSILON_THRESHOLD &&
			!wallTop)
		{
			// maintain state
			jumpBoostRemaining *= SOPHIA_JUMP_BOOST_DECAY;
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
		(input[INPUT_JUMP] == KEY_STATE_ON_DOWN))
	{
		Sound::getInstance()->play("sophia_jump", false, 1);
		newState |= SOPHIA_STATE_JUMPING;
		jumpBoostRemaining = SOPHIA_JUMP_BOOST_AMOUNT;
	}
	//*/

	if ((input[INPUT_LEFT] & KEY_STATE_DOWN) && 
		(input[INPUT_RIGHT] & KEY_STATE_DOWN))
		GoHalt();
	else if (input[INPUT_LEFT] & KEY_STATE_DOWN)
		GoLeft();
	else if (input[INPUT_RIGHT] & KEY_STATE_DOWN)
		GoRight();
	else
		GoHalt();

	if (wallBot && (prevState & SOPHIA_STATE_AIRBORNE))
	{
		newState |= SOPHIA_STATE_LANDING;
	}

	if (wallBot && v.y == 0 && v.x != 0)
	{
		newState |= SOPHIA_STATE_WALKING;
	}
	else {
		newState &= ~SOPHIA_STATE_WALKING;
		EndAnimationType(SOPHIA_ANI_WALKING);
		EndAnimationType(SOPHIA_ANI_LOOKED_UP_WALKING);
	}

	if (!(newState & SOPHIA_STATE_AIRBORNE) &&
		!(prevState & SOPHIA_STATE_LEAVING_VEHICLE) &&
		!(prevState & SOPHIA_STATE_LEFT_VEHICLE) &&
		(input[INPUT_LEAVE_VEHICLE] == KEY_STATE_ON_DOWN))
	{
		newState |= SOPHIA_STATE_LEAVING_VEHICLE;
		jason = new JasonSideView(pos.x + JASON_SPAWN_OFFSET_X, pos.y + JASON_SPAWN_OFFSET_Y);
		jason->sophia = this;
		jason->SetManager(manager);
		jason->v.x = v.x;
		// Set really big velocity
		jason->v.y = -JASON_TINY_JUMP_SPEED;
		//jason->v.y = -10000000;
		manager->AddElement(jason);
		Sound::getInstance()->play("swap_player", false, 1);

		v = Point();
	}
	//*
	if (newState & SOPHIA_ANI_LEFT_VEHICLE)
		StartAnimationType(SOPHIA_ANI_LEFT_VEHICLE);
	//*/
	if (input[INPUT_SHOOT] == KEY_STATE_ON_DOWN) {
		Shoot();
		Sound::getInstance()->play("sophia_shoot", false, 1);
	}


	if (input[INPUT_SHOOT_SPECIAL] == KEY_STATE_ON_DOWN)
	{
		switch ((GameGlobal::GetSpecialBulletType()))
		{
		case 1:
			ShootHoming();
			break;
		case 2:
			ShootThunder();
			break;
		case 3:
			ShootMultiwarheadMissile();
			break;
		default:
			break;
		} 
	}

	if (HealthPoint <= 0)
	{
		newState |= SOPHIA_STATE_DYING;
		// newState |= SOPHIA_STATE_DEAD;
		Sound::getInstance()->play("sophia_explosion", false, 1);
	}

	if (prevState != newState)
	{
		SetState(newState);
		SetAniByState(newState);
	}
	else
		if (newState & (SOPHIA_STATE_LOOKED_UP | SOPHIA_STATE_WALKING | SOPHIA_STATE_HANGING_WHEEL))
		{
			SetAniByState(newState);
		}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Sophia::Render()
{
	Point cameraOffset = Camera::GetInstance()->GetPosition();
	cameraOffset.x -= Camera::GetInstance()->GetWidth() / 2;
	cameraOffset.y -= Camera::GetInstance()->GetHeight() / 2;
	drawArguments.SetPosition(pos - cameraOffset);
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
	if ((state & SOPHIA_STATE_LEAVING_VEHICLE) && currentAni[SOPHIA_ANI_LEAVING_VEHICLE])
		targetAni = SOPHIA_ANI_LEAVING_VEHICLE;
	if (state & SOPHIA_STATE_LEFT_VEHICLE)
		targetAni = SOPHIA_ANI_LEFT_VEHICLE;
	if ((state & SOPHIA_STATE_ENTERING_VEHICLE) && currentAni[SOPHIA_ANI_LEAVING_VEHICLE])
		targetAni = SOPHIA_ANI_LEAVING_VEHICLE;
	//*
	if (currentAni[SOPHIA_ANI_DYING])
		targetAni = SOPHIA_ANI_DYING;//*/

	int* targetFrame = &currentFrame[targetAni];
	targetTime = currentTime[targetAni];
	if (targetAni == SOPHIA_ANI_TURNING)
	{
		targetFrame = &currentFrame[backAni];
		targetTime = *targetFrame * 100 + targetTime % 100;
	}

	if (currentAni[SOPHIA_ANI_WALKING]
		|| currentAni[SOPHIA_ANI_LOOKED_UP_WALKING])
	{
		int frameTotal = animations.size() * SOPHIA_WHEEL_DURATION;
		currentWheelTime = (currentWheelTime + 1) % frameTotal;
		currentSet = currentWheelTime / SOPHIA_WHEEL_DURATION;
	}

	animations[currentSet]->at(targetAni)->
		Render(targetTime, *targetFrame, drawArguments);

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
				if (i == SOPHIA_ANI_DYING)
				{
					manager->RemoveElement(this);
				}
				if (i == SOPHIA_ANI_LEAVING_VEHICLE) {
					if (state & SOPHIA_STATE_LEAVING_VEHICLE) {
						stateToChange.push_back(
							make_pair(SOPHIA_STATE_LEAVING_VEHICLE, false));
						stateToChange.push_back(
							make_pair(SOPHIA_STATE_LEFT_VEHICLE, true));
					}
					if (state & SOPHIA_STATE_ENTERING_VEHICLE) {
						stateToChange.push_back(
							make_pair(SOPHIA_STATE_ENTERING_VEHICLE, false));
						stateToChange.push_back(
							make_pair(SOPHIA_STATE_LEFT_VEHICLE, false));
						if (jason != NULL)
							manager->RemoveElement(jason);
					}
				}
				if (i == SOPHIA_ANI_JUMPING || i == SOPHIA_ANI_LOOKED_UP_JUMPING) {
					if (!(state & SOPHIA_STATE_AIRBORNE))
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
				drawArguments.FlipVertical(state & SOPHIA_STATE_LOOKING_LEFT);
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
	StartAnimationType(SOPHIA_ANI_IDLE);

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
}

void Sophia::GoLeft()
{
	v.x -= SOPHIA_ACCELERATION;

	if (v.x < -SOPHIA_WALKING_SPEED)
		v.x = -SOPHIA_WALKING_SPEED;

	if (wallLeft)
		v.x = 0;
}

void Sophia::GoRight()
{
	v.x += SOPHIA_ACCELERATION;

	if (v.x > SOPHIA_WALKING_SPEED)
		v.x = SOPHIA_WALKING_SPEED;

	if (wallRight)
		v.x = 0;
}

void Sophia::GoHalt()
{
	bool positive = v.x > 0;
	v.x = abs(v.x) - SOPHIA_ACCELERATION;
	if (v.x < 0)
		v.x = 0;
	if (!positive)
		v.x = -v.x;

	if (v.x > 0 && wallRight)
		v.x = 0;
	if (v.x < 0 && wallLeft)
		v.x = 0;
}

void Sophia::Shoot()
{
	if (bullets.size() >= 3)
		return;

	Point bulletV;
	Point bulletOffset;
	if (state & SOPHIA_STATE_LOOKING_LEFT) {
		bulletV = Point(-SOPHIA_BULLET_SPEED, 0);
		bulletOffset = Point(SOPHIA_BULLET_OFFSET_X, SOPHIA_BULLET_OFFSET_Y);
	}
	else {
		bulletV = Point(SOPHIA_BULLET_SPEED, 0);
		bulletOffset = Point(-SOPHIA_BULLET_OFFSET_X, SOPHIA_BULLET_OFFSET_Y);
	}
	if (state & SOPHIA_STATE_LOOKED_UP)
		bulletV = Point(0, -SOPHIA_BULLET_SPEED);
	SophiaBullet* bullet = new SophiaBullet(
		pos + bulletOffset,
		bulletV, 1);

	Managed<GameObject>::manager->AddElement(bullet);
	((Managed<GameObject>*)bullet)->SetManager(Managed<GameObject>::manager);
	((Managed<Bullet>*)bullet)->SetManager(this);
	AddElement(bullet);
}

void Sophia::ShootThunder()
{
	if (GameGlobal::GetNumberSpecialBullet2() <= 0) {
		return;
	}
	Point thunderPos = pos + Point(0, 50);
	int thunderDirX = rand() % 2;
	if (thunderDirX == 0)	thunderDirX = -1;
	ThunderBullet* bullet = new ThunderBullet(thunderPos, 0, thunderDirX, D3DCOLOR(0));
	bullet->SetManager(manager);
	manager->AddElement(bullet);

	GameGlobal::SetSpecialNumberBullet2(GameGlobal::GetNumberSpecialBullet2() - 1);
}

bool Sophia::IsShootingMultiwarhead()
{
	unordered_set<GameObject*> objects;
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		objects = scene->GetObjects();
		for (auto x : objects) {
			if (dynamic_cast<MultiwarheadMissile*>(x) != NULL)
				return true;
		}
		return false;
	}
}

void Sophia::ShootMultiwarheadMissile()
{
	if (GameGlobal::GetNumberSpecialBullet3() <= 0) {
		return;
	}
	if(!IsShootingMultiwarhead()) {
		int dirX;
		if (state & SOPHIA_STATE_LOOKING_LEFT) {
			dirX = -1;
		}
		else {
			dirX = 1;
		}

		for (int i = 1; i <= 3; i++) {
			int numberOfMissile = GameGlobal::GetNumberSpecialBullet3();
			if (numberOfMissile > 0) {
				Point multiwarheadPos = pos;
				MultiwarheadMissile* bullet = new MultiwarheadMissile(multiwarheadPos, dirX, i);
				bullet->SetManager(manager);
				manager->AddElement(bullet);

				GameGlobal::SetSpecialNumberBullet3(GameGlobal::GetNumberSpecialBullet3() - 1);
			}
		}
	}
}
void Sophia::ShootHoming()
{
	if (GameGlobal::GetNumberSpecialBullet1() <= 0) {
		return;
	}
	Point bulletV;
	Point bulletOffset;
	if (state & SOPHIA_STATE_LOOKING_LEFT) {
		bulletV = Point(-SOPHIA_BULLET_SPEED, 0);
		bulletOffset = Point(SOPHIA_BULLET_OFFSET_X, SOPHIA_BULLET_OFFSET_Y);
	}
	else {
		bulletV = Point(SOPHIA_BULLET_SPEED, 0);
		bulletOffset = Point(-SOPHIA_BULLET_OFFSET_X, SOPHIA_BULLET_OFFSET_Y);
	}
	if (state & SOPHIA_STATE_LOOKED_UP)
		bulletV = Point(0, -SOPHIA_BULLET_SPEED);
	HomingBullet* bullet = new HomingBullet(
		pos + bulletOffset,
		bulletV);

	bullet->SetManager(manager);
	manager->AddElement(bullet);

	GameGlobal::SetSpecialNumberBullet1(GameGlobal::GetNumberSpecialBullet1() - 1);
}

bool Sophia::IsPrimaryPlayer()
{
	return !(state & SOPHIA_STATE_LEAVING_VEHICLE) && 
		!(state & SOPHIA_STATE_LEFT_VEHICLE) && 
		!(state & SOPHIA_STATE_ENTERING_VEHICLE);
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
	currentAni[SOPHIA_ANI_IDLE] = true;
	currentSet = 0;
	// set looking right
	drawArguments.FlipVertical(true);

	HealthPoint = SOPHIA_MAX_HEALTH;
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
	currentAni[SOPHIA_ANI_IDLE] = true;
	currentSet = 0;
	// set looking right
	drawArguments.FlipVertical(true);
	HealthPoint = SOPHIA_MAX_HEALTH;
}

void Sophia::SetState(int state)
{
	GameObject::SetState(state);
}

void Sophia::SetAniByState(int state)
{
	if (state & SOPHIA_STATE_DYING)
		StartAnimationType(SOPHIA_ANI_DYING);
	if (state & SOPHIA_STATE_LEFT_VEHICLE)
		StartAnimationType(SOPHIA_ANI_LEFT_VEHICLE);
	if ((state & SOPHIA_STATE_LEAVING_VEHICLE) ||
		(state & SOPHIA_STATE_ENTERING_VEHICLE))
		StartAnimationType(SOPHIA_ANI_LEAVING_VEHICLE);
	if (state & SOPHIA_STATE_LOOKING_UP)
		StartAnimationType(SOPHIA_ANI_LOOKING_UP);
	if (state & SOPHIA_STATE_LOOKED_UP)
	{
		if (state & SOPHIA_STATE_WALKING)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_WALKING);
		if (state & SOPHIA_STATE_JUMPING)
			StartAnimationType(SOPHIA_ANI_LOOKED_UP_JUMPING);
		if ((state & SOPHIA_STATE_JUMP_BOOST) || (state & SOPHIA_STATE_HANGING_WHEEL))
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
		if ((state & SOPHIA_STATE_JUMP_BOOST) || (state & SOPHIA_STATE_HANGING_WHEEL))
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

void Sophia::TakeDamage(int damage)
{
	if (!(state & SOPHIA_STATE_ENTERING_VEHICLE) &&
		!(state & SOPHIA_STATE_LEFT_VEHICLE) &&
		!(state & SOPHIA_STATE_LEAVING_VEHICLE))
		Player::TakeDamage(damage);
}