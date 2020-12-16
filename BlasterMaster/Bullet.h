#pragma once
#include "AnimatedGameObject.h"
#include "Manager.h"

// Note: define for shooting left
#define BULLET_OFFSET_RIGHT 3
#define BULLET_OFFSET_DOWN 2.99
#define BULLET_OFFSET_UP -2.99
#define BULLET_OFFSET_LEFT -3

#define BULLET_ANIMATION_SET_NUMBER 17

enum BulletLevel :int {
	BULLET_LEVEL1			= 1,
};

enum BulletFlag : int {
	BULLET_STATE_EXPLODE	= 0x0001
};

enum BulletAni :int {
	BULLET_ANI_ROUND_FIREBALL	= 0,		// fireball shape
	BULLET_ANI_LONG_FIREBALL	= 1,		// enlongated fireball shape
	BULLET_ANI_ORB_SMALL		= 2,		// orb small
	BULLET_ANI_ORB_MEDIUM		= 3,		// orb medium
	BULLET_ANI_ORB_LARGE		= 4,		// orb large
	BULLET_ANI_GRENADE			= 5,		// grenade
	BULLET_ANI_EXPLODE			= 6,
	BULLET_ANI_GRENADE_FRAG		= 7			// grenade fragment
};

enum BulletDamageModifier : int {
	BULLET_MODIFIER_NONE				= 0,
	BULLET_MODIFIER_BREAKABLE_WALL		= 1,
	BULLET_MODIFIER_JASON_VULNERABLE	= 2,
};

enum BulletDirection : int {
	BULLET_DIR_LEFT			= 0,
	BULLET_DIR_UP			= 1,
	BULLET_DIR_RIGHT		= 2,
	BULLET_DIR_DOWN			= 3,
};

class Bullet :
	public AnimatedGameObject
{
protected:
	int type;
	BulletDirection dir;
public:

	Bullet();
	Bullet(Point pos, Point v, int type);
	virtual ~Bullet();

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();

	virtual void Render();
	virtual void SetAnimationSet(AnimationSet* aniSet);

	int GetLevel();
	virtual int GetDamage(BulletDamageModifier modifier) = 0;
	virtual int GetDamage() { return GetDamage(BULLET_MODIFIER_NONE); }
};

class PlayerBullet : public Bullet
{
public:
	PlayerBullet(Point pos, Point v, int type) : Bullet(pos, v, type) {}
	virtual ~PlayerBullet() {}

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
};
class EnemyBullet : public Bullet
{
public:
	EnemyBullet(Point pos, Point v, int type) : Bullet(pos, v, type) {}
	virtual ~EnemyBullet() {}

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
};

class SophiaBullet : public PlayerBullet
{
	int level;
public:
	SophiaBullet(Point pos, Point v, int level = 1);
	virtual ~SophiaBullet();

	// Inherited via Bullet
	virtual int GetDamage(BulletDamageModifier modifier) override;
};

class TimedPlayerBullet :public PlayerBullet
{
protected:
	// time to live
	int TTL;
public:
	TimedPlayerBullet(Point pos, Point v, int type) : PlayerBullet(pos, v, type) {}
	virtual ~TimedPlayerBullet();

	virtual void Update();
};

#define JASON_SIDEVIEW_BULLET_TIME_TO_LIVE 30
class JasonSideviewBullet : public TimedPlayerBullet
{
public:
	JasonSideviewBullet(Point pos, Point v);
	virtual ~JasonSideviewBullet();

	// Inherited via TimedPlayerBullet
	virtual int GetDamage(BulletDamageModifier modifier) override;
};

#define JASON_OVERHEAD_BULLET_NORM_TIME_TO_LIVE 300
class JasonOverheadBulletNorm : public TimedPlayerBullet, Managed<Bullet>
{
public:
	JasonOverheadBulletNorm(Point pos, Point v);
	virtual ~JasonOverheadBulletNorm();

	// Inherited via TimedPlayerBullet
	virtual int GetDamage(BulletDamageModifier modifier) override;
};

#define JASON_OVERHEAD_GRENADE_TIME_TO_LIVE 10

class JasonOverheadBulletGrenade : public TimedPlayerBullet
{
public:
	JasonOverheadBulletGrenade(Point pos, Point v);
	virtual ~JasonOverheadBulletGrenade();

	// Inherited via TimedPlayerBullet
	virtual int GetDamage(BulletDamageModifier modifier) override;

	virtual BoundingBox GetBoundingBox();
	virtual void Render();
};

#define JASON_OVERHEAD_GRENADE_FRAGMENT_BBOX_LEFT 10
#define JASON_OVERHEAD_GRENADE_FRAGMENT_BBOX_TOP 10
#define JASON_OVERHEAD_GRENADE_FRAGMENT_BBOX_RIGHT 10
#define JASON_OVERHEAD_GRENADE_FRAGMENT_BBOX_BOTTOM 10
class JasonOverheadBulletGrenadeFragment : public PlayerBullet
{
	int damage;
public:
	JasonOverheadBulletGrenadeFragment(Point pos, int damage);
	virtual ~JasonOverheadBulletGrenadeFragment();

	// Inherited via TimedPlayerBullet
	virtual int GetDamage(BulletDamageModifier modifier) override;
};

#define THUNDER_BBOX_OFFSET_LEFT	   -16 + 1
#define THUNDER_BBOX_OFFSET_RIGHT		16 - 1
#define THUNDER_BBOX_OFFSET_TOP		   -32 + 1
#define THUNDER_BBOX_OFFSET_BOTTOM		32 - 1

class ThunderBullet : public PlayerBullet
{
	int dirX;
	bool isCreateAnotherThunder;

	void SetRandomColor();
public:
	int numberOfThunder;
	ThunderBullet(Point pos, int numberOfThunder, int dirX, D3DCOLOR color);
	//virtual void SetState(int state);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
	virtual int GetDamage(BulletDamageModifier modifier) override;
};