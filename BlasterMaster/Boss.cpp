#include "Boss.h"
#include "Utils.h"
#include "Game.h"
#include "Sound.h"
#include "GameGlobal.h"

#define NUMBER_ARM_MOVEMENT	12
#define MAX_ROW_BOSS_REACH 90

static Point targetDirectionOffset[NUMBER_ARM_MOVEMENT] = { Point(-40, 50), Point(40,50), Point(-80, 90), Point(80,90), Point(20,-30), Point(40,-20), Point(0,100), Point(0,-100), Point(-40,-20), Point(-20,30), Point(90,-100), Point(-90,100) };

Boss::Boss() {
	pos = Point();
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	leftArmTargetDirectionEnum = 0;
	rightArmTargetDirectionEnum = 1;
	HealthPoint = BOSS_HEALTHPOINT;
	v = Point(0.1, 0.06);
}

Boss::Boss(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	leftArmTargetDirectionEnum = 0;
	rightArmTargetDirectionEnum = 1;
	//SetState();
	HealthPoint = BOSS_HEALTHPOINT;
	v = Point(0.1, 0.06);
}

Boss::~Boss()
{
	for (int i = 0; i < leftArm.size(); i++)
	{
		manager->RemoveElement(leftArm[i]);
		manager->RemoveElement(rightArm[i]);
	}
}

BoundingBox Boss::GetBoundingBox()
{
	float left = pos.x + BODY_BBOX_OFFSET_LEFT;
	float top = pos.y + BODY_BBOX_OFFSET_TOP;
	float right = pos.x + BODY_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + BODY_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Boss::Update()
{
	pos += dx();
	Enemy::Update();

	// when interact wall
	if (wallLeft) {
		v.x = BOSS_MOVING_SPEED;
	}
	else if (wallRight) {
		v.x = -BOSS_MOVING_SPEED;
	}
	
	if (wallTop) {
		v.y = BOSS_MOVING_SPEED;
	}
	else if (pos.y > MAX_ROW_BOSS_REACH) {
		v.y = -BOSS_MOVING_SPEED;
	}

	leftArmShoulder = Point(pos.x - BOSS_SHOULDER_OFFSET_X, pos.y + BOSS_SHOULDER_OFFSET_Y);
	rightArmShoulder = Point(pos.x + BOSS_SHOULDER_OFFSET_X, pos.y + BOSS_SHOULDER_OFFSET_Y);

	// If Arms are not initialized, make them
	if (leftArm.size() <= 0)
	{
		BossArm* temp;
		for (int i = 0; i < BOSS_ARM_AMOUNT; i++)
		{
			temp = new BossArm(leftArmShoulder.x, leftArmShoulder.y);
			leftArm.push_back(temp);
			manager->AddElement(temp);
			temp->SetManager(manager);

			temp = new BossArm(rightArmShoulder.x, rightArmShoulder.y);
			rightArm.push_back(temp);
			manager->AddElement(temp);
			temp->SetManager(manager);
		}
		BossClaw* temp2 = new BossClaw(leftArmShoulder.x, leftArmShoulder.y);
		temp2->SetDelayBetweenTargets(BOSS_CLAW_REACH_TARGET_DELAY_LEFT);
		leftArm.push_back(temp2);
		manager->AddElement(temp2);
		temp2->SetManager(manager);

		temp2 = new BossClaw(rightArmShoulder.x, rightArmShoulder.y);
		temp2->SetDelayBetweenTargets(BOSS_CLAW_REACH_TARGET_DELAY_RIGHT);
		rightArm.push_back(temp2);
		manager->AddElement(temp2);
		temp2->SetManager(manager);
	}

	// Move the claw at the end of left & right arm
	if (leftArm[BOSS_ARM_AMOUNT]->SetTargetLocation(leftArmShoulder + targetDirectionOffset[leftArmTargetDirectionEnum]))
	{
		leftArmTargetDirectionEnum++;
		leftArmTargetDirectionEnum %= NUMBER_ARM_MOVEMENT;
	}
	if (rightArm[BOSS_ARM_AMOUNT]->SetTargetLocation(rightArmShoulder + targetDirectionOffset[rightArmTargetDirectionEnum]))
	{
		rightArmTargetDirectionEnum++;
		rightArmTargetDirectionEnum %= NUMBER_ARM_MOVEMENT;
	}

	// Set TargetLocation for Arms
	leftArm[0]->SetTargetLocation((leftArm[1]->pos + leftArmShoulder) * 0.5);
	rightArm[0]->SetTargetLocation((rightArm[1]->pos + rightArmShoulder) * 0.5);
	for (int i = 1; i < BOSS_ARM_AMOUNT; i++)
	{
		leftArm[i]->SetTargetLocation((leftArm[i - 1]->pos + leftArm[i + 1]->pos) * 0.5);
		rightArm[i]->SetTargetLocation((rightArm[i - 1]->pos + rightArm[i + 1]->pos) * 0.5);
	}

	// bullet
	if (timeToShoot == 0) {
		// nếu còn lượt thì bắn, trừ lượt và đặt lại time cho viên tiếp theo
		if (shootTurn != 0) {
			Shoot();
			shootTurn--;
			timeToShoot = 30;
		}
		else {
			shootTurn = rand() % 1 + 4;
			timeToShoot = 250;
		}
	}
	timeToShoot--;

	
	for (int i = 0; i < leftArm.size(); i++)
	{
		leftArm[i]->UpdateOverloaded();
		rightArm[i]->UpdateOverloaded();
	}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Boss::Render()
{
	if (currentAnimation == NULL)
		SetAnimationType(0);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Boss::TakeDamage(int damage)
{
	Enemy::TakeDamage(damage);
	DebugOut(L"Boss HP: %d \n", HealthPoint);
}

void Boss::Shoot()
{
	Point direction = targetPlayer - pos;
	Point v = direction * (BOSS_BULLET_SPEED / direction.length());

	BossBullet* bullet = new BossBullet(pos, v);
	
	bullet->SetManager(manager);
	manager->AddElement(bullet);
}

void Boss::SetState(int state)
{
	GameObject::SetState(state);

}

BossArm::BossArm()
{
	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(BOSS_ARM_ANIMATION_SET_NUMBER));
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetAnimationType(BOSS_ARM_ANI);
}

BossArm::BossArm(float x, float y)
{
	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(BOSS_ARM_ANIMATION_SET_NUMBER));
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetAnimationType(BOSS_ARM_ANI);
}

BoundingBox BossArm::GetBoundingBox()
{
	float left = pos.x + BOSS_ARM_BBOX_OFFSET_LEFT;
	float top = pos.y + BOSS_ARM_BBOX_OFFSET_TOP;
	float right = pos.x + BOSS_ARM_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + BOSS_ARM_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void BossArm::Update()
{
	// Do not call Enemy::Update()
}

void BossArm::UpdateOverloaded()
{
	pos += dx();
	Enemy::Update();
	v = (targetLocation - pos) * (1.0 / BOSS_ARM_TIME_TO_REACH_TARGET_LOCATION);

	// If rounding problems cause v to lose all value
	if (v == 0)
		v = targetLocation - pos;

	HealthPoint = MAXINT / 2;
}

bool BossArm::SetTargetLocation(Point trg)
{
	targetLocation = trg;
	return true;
}

BossClaw::BossClaw()
{
	SetAnimationType(BOSS_CLAW_ANI);
}

BossClaw::BossClaw(float x, float y) :BossArm(x, y)
{
	SetAnimationType(BOSS_CLAW_ANI);
}

BoundingBox BossClaw::GetBoundingBox()
{
	float left = pos.x + BOSS_CLAW_BBOX_OFFSET_LEFT;
	float top = pos.y + BOSS_CLAW_BBOX_OFFSET_TOP;
	float right = pos.x + BOSS_CLAW_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + BOSS_CLAW_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

bool BossClaw::SetTargetLocation(Point trg)
{
	if (countdownUntilNewTarget <= 0) {
		BossArm::SetTargetLocation(trg);
		return true;
	}
	return false;
}

void BossClaw::UpdateOverloaded()
{
	BossArm::UpdateOverloaded();

	if (pos == targetLocation) {
		countdownUntilNewTarget--;

		v = Point();
	}
	else
	{
		countdownUntilNewTarget = delayBetweenTargets;

		Point proposedV = targetLocation - pos;
		double proposedSpeed = proposedV.length();
		v = proposedV * (min(BOSS_CLAW_MAX_SPEED, proposedSpeed) / proposedSpeed);
	}
}

void BossClaw::SetDelayBetweenTargets(int time)
{
	delayBetweenTargets = time;
}
