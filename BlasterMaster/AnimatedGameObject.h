#pragma once
#include "GameObject.h"
class AnimatedGameObject : public GameObject
{
protected:
	//Long
	//int currentFrame;
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
	bool isFlipVertical = false;
	bool isFlipHorizontal = false;
	float rotation = 0;
public:
	virtual void Render() override;
	virtual void SetAnimationType(int ANI);

	AnimatedGameObject() {}
	virtual ~AnimatedGameObject() {}
};

#define DURATION_OF_DAMAGE_FLASH 100
#define ENEMY_SPRITE_DURATION_OF_DAMAGE_FLASH 10
class Enemy : public AnimatedGameObject
{
public:
	Enemy() { HealthPoint = 20; }
	virtual ~Enemy() {}

	virtual void TakeDamage(int damage);
	virtual void Update();
	int damageFrame;
	Point direction;
	bool isCollided; // true when enemy collide with player
};


class AnimatedScene : public AnimatedGameObject
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
};
