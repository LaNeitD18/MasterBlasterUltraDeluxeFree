#include "Boss.h"
#include "Utils.h"
#include "Game.h"
#include "Sound.h"
#include "GameGlobal.h"

static Point targetDirectionOffset[4] = { Point(-40, 50),Point(40,50), Point(40,-20), Point(-40,-20) };

Boss::Boss() {
	pos = Point();
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	leftArmTargetDirectionEnum = 0;
	rightArmTargetDirectionEnum = 1;
	HealthPoint = BOSS_HEALTHPOINT;
}

Boss::Boss(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	leftArmTargetDirectionEnum = 0;
	rightArmTargetDirectionEnum = 1;
	//SetState();
	HealthPoint = BOSS_HEALTHPOINT;
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
	//pos += dx();
	Enemy::Update();

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
		leftArmTargetDirectionEnum %= 4;
	}
	if (rightArm[BOSS_ARM_AMOUNT]->SetTargetLocation(rightArmShoulder + targetDirectionOffset[rightArmTargetDirectionEnum]))
	{
		rightArmTargetDirectionEnum++;
		rightArmTargetDirectionEnum %= 4;
	}

	// Set TargetLocation for Arms
	leftArm[0]->SetTargetLocation((leftArm[1]->pos + leftArmShoulder) * 0.5);
	rightArm[0]->SetTargetLocation((rightArm[1]->pos + rightArmShoulder) * 0.5);
	for (int i = 1; i < BOSS_ARM_AMOUNT; i++)
	{
		leftArm[i]->SetTargetLocation((leftArm[i - 1]->pos + leftArm[i + 1]->pos) * 0.5);
		rightArm[i]->SetTargetLocation((rightArm[i - 1]->pos + rightArm[i + 1]->pos) * 0.5);
	}
	
	for (int i = 0; i < leftArm.size(); i++)
	{
		leftArm[i]->UpdateOverloaded();
		rightArm[i]->UpdateOverloaded();
	}


	v = Point();
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
