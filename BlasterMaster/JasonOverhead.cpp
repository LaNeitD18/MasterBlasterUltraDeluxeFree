#include "JasonOverhead.h"
#include "GameGlobal.h"
#include "Utils.h"

JasonOverhead::JasonOverhead()
{
	state = 0x01;
	pos = Point();

	currentAnimation = NULL;
	HealthPoint = JASONO_MAX_HEALTH;

	/*
	bulletPower = JASONO_MAX_BULLET_POWER;
	/*/
	bulletPower = 0;
	//*/
}
JasonOverhead::JasonOverhead(float x, float y)
{
	pos = Point(x, y);

	SetState(JASONO_STATE_LOOKING_UP);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	//SetAnimationType(0);
	// set looking right
	//drawArguments.FlipVertical(true);
	HealthPoint = JASONO_MAX_HEALTH;

	/*
	bulletPower = JASONO_MAX_BULLET_POWER;
	/*/
	bulletPower = 0;
	//*/
}

JasonOverhead::~JasonOverhead()
{
}

void JasonOverhead::TakeDamage(int damage)
{
	if (invulnerableFrame <= 0 && HealthPoint > 0 && damage > 0)
	{
		bulletPower -= JASONO_POWER_MIN_STEP;
		if (bulletPower < 0)
			bulletPower = 0;
	}

	Player::TakeDamage(damage);
}

BoundingBox JasonOverhead::GetBoundingBox()
{
	return BoundingBox(
		pos.x + JASONO_BBOX_OFFSET_LEFT,
		pos.y + JASONO_BBOX_OFFSET_TOP,
		pos.x + JASONO_BBOX_OFFSET_RIGHT,
		pos.y + JASONO_BBOX_OFFSET_BOTTOM);
}

void JasonOverhead::Render()
{
	if (state & JASONO_STATE_LOOKING_UP)
		SetAnimationType(JASONO_ANI_GO_UP);
	else if (state & JASONO_STATE_LOOKING_DOWN)
		SetAnimationType(JASONO_ANI_GO_DOWN);
	else if (state & JASONO_STATE_DEAD)
		SetAnimationType(JASONO_ANI_DEAD);
	else if (state & JASONO_STATE_DYING) {
		SetAnimationType(JASONO_ANI_DYING);
	}
	else {
		if (state & JASONO_STATE_LOOKING_LEFT)
			isFlipVertical = false;
		else
			isFlipVertical = true;
		SetAnimationType(JASONO_ANI_WALK);
	}

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
	if(v.x != 0 || v.y != 0 || state == JASONO_STATE_DYING)
		currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		if (currentAnimation == animationSet->at(JASONO_ANI_DYING))
			SetState(JASONO_STATE_DEAD);
		/*else if (currentAnimation == animationSet->at(JASONO_ANI_DEAD))
			manager->RemoveElement(this);*/
		else {
			currentTime %= currentAnimation->GetLoopDuration();
			previousFrame = 0;
		}
	}
	else if (currentTime == 0 && state == JASONO_STATE_DEAD)
		manager->RemoveElement(this);
}

void JasonOverhead::Update()
{
	pos += dx();
	Player::Update();
	Input& input = *GameGlobal::GetInput();

	int newState = state;
	int flags = 0;
	bool dead = (state == JASONO_STATE_DYING) ||
		(state == JASONO_STATE_DEAD);

	if (wallTop && (v.y < 0))
		v.y = 0;

	if (!dead) {
		if (!(input[INPUT_LEFT] & KEY_STATE_DOWN) ^
			(input[INPUT_RIGHT] & KEY_STATE_DOWN)) {
			GoHaltHorizontal();
		}
		else if (input[INPUT_LEFT] & KEY_STATE_DOWN) {
			GoLeft();

			if (!(input[INPUT_UP] & KEY_STATE_DOWN) ^ (input[INPUT_DOWN] & KEY_STATE_DOWN)) {
				newState |= JASONO_STATE_LOOKING_LEFT;
				newState &= ~JASONO_STATE_LOOKING_RIGHT;
				newState &= ~JASONO_STATE_LOOKING_UP;
				newState &= ~JASONO_STATE_LOOKING_DOWN;
			}
		}
		else if (input[INPUT_RIGHT] & KEY_STATE_DOWN) {
			GoRight();

			if (!(input[INPUT_UP] & KEY_STATE_DOWN) ^ (input[INPUT_DOWN] & KEY_STATE_DOWN)) {
				newState &= ~JASONO_STATE_LOOKING_LEFT;
				newState |= JASONO_STATE_LOOKING_RIGHT;
				newState &= ~JASONO_STATE_LOOKING_UP;
				newState &= ~JASONO_STATE_LOOKING_DOWN;
			}
		}

		if (!(input[INPUT_UP] & KEY_STATE_DOWN) ^
			(input[INPUT_DOWN] & KEY_STATE_DOWN))
			GoHaltVertical();
		else if (input[INPUT_UP] & KEY_STATE_DOWN) {
			GoUp();

			if (!(input[INPUT_RIGHT] & KEY_STATE_DOWN) ^ (input[INPUT_LEFT] & KEY_STATE_DOWN)) {
				newState &= ~JASONO_STATE_LOOKING_LEFT;
				newState &= ~JASONO_STATE_LOOKING_RIGHT;
				newState |= JASONO_STATE_LOOKING_UP;
				newState &= ~JASONO_STATE_LOOKING_DOWN;
			}
		}
		else if (input[INPUT_DOWN] & KEY_STATE_DOWN) {
			GoDown();

			if (!(input[INPUT_RIGHT] & KEY_STATE_DOWN) ^ (input[INPUT_LEFT] & KEY_STATE_DOWN)) {
				newState &= ~JASONO_STATE_LOOKING_LEFT;
				newState &= ~JASONO_STATE_LOOKING_RIGHT;
				newState &= ~JASONO_STATE_LOOKING_UP;
				newState |= JASONO_STATE_LOOKING_DOWN;
			}
		}
	}
	if (input[INPUT_JUMP] == KEY_STATE_ON_DOWN)
	{
		ShootGrenade();
	}
	else if (input[INPUT_SHOOT] == KEY_STATE_ON_DOWN)
	{
		ShootNorm();
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

	wallBot = wallLeft = wallRight = wallTop = false;

	if (dead) {
		v = Point();
		DebugOut(L"Dead");
		return;
	}
		
	if (newState != state)
		SetState(newState);

	if (HealthPoint <= 0 && state != JASONO_STATE_DEAD) {
		SetState(JASONO_STATE_DYING);
	}
	
	
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
	v.x -= JASONO_ACCELERATION;

	if (v.x < -JASONO_WALKING_SPEED)
		v.x = -JASONO_WALKING_SPEED;

	if (wallLeft)
		v.x = 0;
}

void JasonOverhead::GoRight()
{
	v.x += JASONO_ACCELERATION;

	if (v.x > JASONO_WALKING_SPEED)
		v.x = JASONO_WALKING_SPEED;

	if (wallRight)
		v.x = 0;
}

void JasonOverhead::GoUp()
{
	v.y -= JASONO_ACCELERATION;

	if (v.y < -JASONO_WALKING_SPEED)
		v.y = -JASONO_WALKING_SPEED;

	if (wallTop)
		v.y = 0;
}

void JasonOverhead::GoDown()
{
	v.y += JASONO_ACCELERATION;

	if (v.y > JASONO_WALKING_SPEED)
		v.y = JASONO_WALKING_SPEED;

	if (wallBot)
		v.y = 0;
}

void JasonOverhead::GoHaltHorizontal()
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

void JasonOverhead::GoHaltVertical()
{
	bool positive = v.y > 0;
	v.y = abs(v.y) - JASONO_ACCELERATION;
	if (v.y < 0)
		v.y = 0;
	if (!positive)
		v.y = -v.y;

	if (v.y > 0 && wallRight)
		v.y = 0;
	if (v.y < 0 && wallRight)
		v.y = 0;
}

void JasonOverhead::ShootNorm()
{
	Point v;
	if (state & JASONO_STATE_LOOKING_RIGHT)
		v = Point(JASONO_BULLET_SPEED, 0);
	else if (state & JASONO_STATE_LOOKING_LEFT)
		v = Point(-JASONO_BULLET_SPEED, 0);
	else if (state & JASONO_STATE_LOOKING_UP)
		v = Point(0, -JASONO_BULLET_SPEED);
	else if (state & JASONO_STATE_LOOKING_DOWN)
		v = Point(0, JASONO_BULLET_SPEED);

	if (bulletPower >= JASONO_SINE_BULLET_POWER_THRESHOLD)
	{
		// Sine bullet
	}
	else
	{
		if (bullets.size() < 2) {
			JasonOverheadBulletNorm* bullet = new JasonOverheadBulletNorm
				(GetBoundingBox().GetCenter(), v, 
				(float)bulletPower / JASONO_MAX_BULLET_POWER);

			Managed<GameObject>::manager->AddElement(bullet);
			((Managed<GameObject>*)bullet)->SetManager(Managed<GameObject>::manager);
			((Managed<Bullet>*)bullet)->SetManager(this);
			AddElement(bullet);
		}
	}
}

// Note: button 'x'
void JasonOverhead::ShootGrenade()
{
	Point v;
	if (state & JASONO_STATE_LOOKING_UP)
		v = Point(0, -JASONO_GRENADE_SPEED);

	if (state & JASONO_STATE_LOOKING_DOWN)
		v = Point(0, JASONO_GRENADE_SPEED);

	if (state & JASONO_STATE_LOOKING_LEFT) {
		if (isFlipVertical)
			v = Point(JASONO_GRENADE_SPEED, 0);
		else v = Point(-JASONO_GRENADE_SPEED, 0);
	}

	if (bullets.size() < 2) {
		JasonOverheadBulletGrenade* bullet = new JasonOverheadBulletGrenade
			(GetBoundingBox().GetCenter(), v, 
			(float)bulletPower / JASONO_MAX_BULLET_POWER);

		Managed<GameObject>::manager->AddElement(bullet);
		((Managed<GameObject>*)bullet)->SetManager(Managed<GameObject>::manager);
		((Managed<Bullet>*)bullet)->SetManager(this);
		AddElement(bullet);
	}
}