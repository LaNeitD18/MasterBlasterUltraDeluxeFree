#pragma once
#include "GameObject.h"
#include "Manager.h"
#include <unordered_set>

#define INPUT_JUMP				'X'
#define INPUT_LEFT				VK_LEFT
#define INPUT_RIGHT				VK_RIGHT
#define INPUT_UP				VK_UP
#define INPUT_DOWN				VK_DOWN
#define INPUT_LEAVE_VEHICLE		VK_SHIFT
#define INPUT_SHOOT				'Z'

#define DURATION_OF_INVULNERABILITY 100
#define SPRITE_DURATION_OF_INVULNERABILTY 10

using std::unordered_set;

class Player : public GameObject, public Manager<Bullet>
{
public:
	unordered_set<Bullet*> bullets;
	int invulnerableFrame;
	virtual void TakeDamage(int damage);
	virtual void Update() override;

	virtual bool IsPrimaryPlayer();

	virtual ~Player() {}
	Player() {}

	// Inherited via Manager
	virtual void AddElement(Bullet *) override;
	virtual void RemoveElement(Bullet *) override;
};
