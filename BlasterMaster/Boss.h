#pragma once
#include "AnimatedGameObject.h"
#include "Manager.h"
#include <vector>

#define BODY_BBOX_OFFSET_LEFT	   -60 + 1
#define BODY_BBOX_OFFSET_RIGHT		60 - 1
#define BODY_BBOX_OFFSET_TOP	   -66 + 1
#define BODY_BBOX_OFFSET_BOTTOM		66 - 1

#define BOSS_ARM_AMOUNT				4
// BOSS_ARM_AMOUNT doesn't count the claws
#define BOSS_CLAW_REACH_TARGET_DELAY_LEFT	40
#define BOSS_CLAW_REACH_TARGET_DELAY_RIGHT	33

#define BOSS_SHOULDER_OFFSET_X	30
#define BOSS_SHOULDER_OFFSET_Y	-0

class BossArm;
class BossClaw;

class Boss : public Enemy
{
public:
	Boss();
	Boss(float x, float y);
	virtual ~Boss();
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
private:
	std::vector<BossArm*> leftArm;
	std::vector<BossArm*> rightArm;
	Point leftArmShoulder;
	Point rightArmShoulder;

	int leftArmTargetDirectionEnum;
	int rightArmTargetDirectionEnum;
};

#define BOSS_ARM_ANIMATION_SET_NUMBER	300
#define BOSS_ARM_TIME_TO_REACH_TARGET_LOCATION 2

#define BOSS_ARM_BBOX_OFFSET_LEFT	    -8
#define BOSS_ARM_BBOX_OFFSET_RIGHT		8
#define BOSS_ARM_BBOX_OFFSET_TOP	    -8
#define BOSS_ARM_BBOX_OFFSET_BOTTOM		8

enum BossArmAni {
	BOSS_ARM_ANI		= 0,
	BOSS_CLAW_ANI		= 1
};

class BossArm : public Enemy // Manager<BossArm>
{
protected:
	Point targetLocation;
public:
	BossArm();
	BossArm(float x, float y);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();

	// Update will be managed by Boss instead of Scene
	virtual void Update();
	// Custom Update, managed by Boss
	virtual void UpdateOverloaded();
	virtual bool SetTargetLocation(Point trg);
};

#define BOSS_CLAW_BBOX_OFFSET_LEFT	    -9
#define BOSS_CLAW_BBOX_OFFSET_RIGHT		9
#define BOSS_CLAW_BBOX_OFFSET_TOP	    -16
#define BOSS_CLAW_BBOX_OFFSET_BOTTOM	16

#define BOSS_CLAW_MAX_SPEED				1.0f
class BossClaw : public BossArm {
	int countdownUntilNewTarget;
	int delayBetweenTargets;
public:
	BossClaw();
	BossClaw(float x, float y);

	virtual BoundingBox GetBoundingBox();
	virtual bool SetTargetLocation(Point trg) override;
	virtual void UpdateOverloaded() override;

	void SetDelayBetweenTargets(int time);
};