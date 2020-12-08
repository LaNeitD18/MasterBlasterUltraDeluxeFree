#include "Bullet.h"
#include "Utils.h"
#include "Camera.h"
#include "GameGlobal.h"
#define _USE_MATH_DEFINES
#include <math.h>

Bullet::Bullet()
{
}

Bullet::Bullet(Point pos, Point v, int type)
{

	this->pos = pos;
	this->v = v;

	state = 0;
	this->type = type;

	if (v.y < 0) {
		rotation = M_PI_2;
		isFlipVertical = false;
	}
	else if (v.y > 0)
	{
		rotation = M_PI_2;
		isFlipVertical = true;
	}
	else if (v.x > 0)
		isFlipVertical = true;
	else
		isFlipVertical = false;

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(BULLET_ANIMATION_SET_NUMBER));

	if (v.y == 0)
		if (v.x < 0)
			dir = BULLET_DIR_LEFT;
		else
			dir = BULLET_DIR_RIGHT;
	else
		if (v.y < 0)
			dir = BULLET_DIR_UP;
		else
			dir = BULLET_DIR_DOWN;
}

Bullet::~Bullet()
{
}

BoundingBox Bullet::GetBoundingBox()
{
	switch (dir)
	{
	case BULLET_DIR_LEFT:
		return BoundingBox(
			pos.x + BULLET_OFFSET_LEFT,
			pos.y + BULLET_OFFSET_UP,
			pos.x + BULLET_OFFSET_RIGHT,
			pos.y + BULLET_OFFSET_DOWN);
		break;
	case BULLET_DIR_UP:
		return BoundingBox(
			pos.x + BULLET_OFFSET_UP,
			pos.y + BULLET_OFFSET_LEFT,
			pos.x + BULLET_OFFSET_DOWN,
			pos.y + BULLET_OFFSET_RIGHT);
		break;
	case BULLET_DIR_RIGHT:
		return BoundingBox(
			pos.x - BULLET_OFFSET_RIGHT,
			pos.y + BULLET_OFFSET_UP,
			pos.x - BULLET_OFFSET_LEFT,
			pos.y + BULLET_OFFSET_DOWN);
		break;
	case BULLET_DIR_DOWN:
		return BoundingBox(
			pos.x + BULLET_OFFSET_DOWN,
			pos.y + BULLET_OFFSET_LEFT,
			pos.x + BULLET_OFFSET_UP,
			pos.y + BULLET_OFFSET_RIGHT);
		break;
	default:
		return BoundingBox();
		break;
	}
}

void Bullet::Update()
{
	pos += dx();

	if (state & BULLET_STATE_EXPLODE)
		v = Point();

	if (!Camera::GetInstance()->GetBound().IsInsideBox(pos))
		SetState(state | BULLET_STATE_EXPLODE);

	if (state & BULLET_STATE_EXPLODE)
		SetAnimationType(BULLET_ANI_EXPLODE);
}

void Bullet::Render()
{
	AnimatedGameObject::Render();
	if (currentTime == 0 && state == BULLET_STATE_EXPLODE)
		manager->RemoveElement(this);
}

void Bullet::SetAnimationSet(AnimationSet* aniSet)
{
	AnimatedGameObject::SetAnimationSet(aniSet);

	switch (type)
	{
	case 0:
		SetAnimationType(BULLET_ANI_ROUND_FIREBALL);
		break;
	case 1:
		SetAnimationType(BULLET_ANI_LONG_FIREBALL);
		break;
	case 2:
		SetAnimationType(BULLET_ANI_ORB_SMALL);
		break;
	case 3:
		SetAnimationType(BULLET_ANI_ORB_MEDIUM);
		break;
	case 4:
		SetAnimationType(BULLET_ANI_ORB_LARGE);
		break;
	case 5:
		SetAnimationType(BULLET_ANI_GRENADE);
		break;
	default:
		//SetAnimationType(BULLET_ANI_NORM1);
		DEBUG(throw 1);
		DebugOut(L"Unknown bullet level: %d", type);
		break;
	}
}

int Bullet::GetLevel() { return type; }

SophiaBullet::SophiaBullet(Point pos, Point v, int level) : PlayerBullet(pos, v, BULLET_ANI_LONG_FIREBALL)
{
	this->level = level;
}

SophiaBullet::~SophiaBullet()
{
}

int SophiaBullet::GetDamage(BulletDamageModifier modifier)
{
	switch (modifier)
	{
	case BULLET_MODIFIER_BREAKABLE_WALL:
		if (level >= 2)
			return 10;
		else return 0;
	case BULLET_MODIFIER_NONE:
		return 10;
	default:
		DebugOut(L"Unkwown bullet modifier: %d", (int)modifier);
		DEBUG(throw 1);
		return 0;
	}
}

JasonSideviewBullet::JasonSideviewBullet(Point pos, Point v) : TimedPlayerBullet(pos, v, BULLET_ANI_ORB_SMALL)
{
	TTL = JASON_SIDEVIEW_BULLET_TIME_TO_LIVE;
}

JasonSideviewBullet::~JasonSideviewBullet()
{
}

int JasonSideviewBullet::GetDamage(BulletDamageModifier modifier)
{
	switch (modifier)
	{
	case BULLET_MODIFIER_BREAKABLE_WALL:
		return 0;
	case BULLET_MODIFIER_NONE:
		return 7;
	default:
		DebugOut(L"Unkwown bullet modifier: %d", (int)modifier);
		DEBUG(throw 1);
		return 0;
	}
}

JasonOverheadBulletNorm::JasonOverheadBulletNorm(Point pos, Point v) : TimedPlayerBullet(pos, v, BULLET_ANI_ROUND_FIREBALL)
{
	TTL = JASON_OVERHEAD_BULLET_NORM_TIME_TO_LIVE;
}

JasonOverheadBulletNorm::~JasonOverheadBulletNorm()
{
}

int JasonOverheadBulletNorm::GetDamage(BulletDamageModifier modifier)
{
	// TODO: Implement this
	DEBUG(throw 1);
	return 0;
}

JasonOverheadBulletGrenade::JasonOverheadBulletGrenade(Point pos, Point v) : TimedPlayerBullet(pos, v, BULLET_ANI_GRENADE)
{
	TTL = JASON_OVERHEAD_GRENADE_TIME_TO_LIVE;
}

JasonOverheadBulletGrenade::~JasonOverheadBulletGrenade()
{
}

int JasonOverheadBulletGrenade::GetDamage(BulletDamageModifier modifier)
{
	// Yes, don't do damage. Fragments do damage.
	return 0;
}

BoundingBox JasonOverheadBulletGrenade::GetBoundingBox()
{
	return BoundingBox();
}

void JasonOverheadBulletGrenade::Render()
{
	AnimatedGameObject::Render();
	if (currentTime == 0 && state == BULLET_STATE_EXPLODE) {

		manager->RemoveElement(this);
	}
}

TimedPlayerBullet::~TimedPlayerBullet()
{
}

void TimedPlayerBullet::Update()
{
	TTL--;
	if (TTL <= 0)
		SetState(state | BULLET_STATE_EXPLODE);

	PlayerBullet::Update();
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS PlayerBullet
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS EnemyBullet
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS

JasonOverheadBulletGrenadeFragment::JasonOverheadBulletGrenadeFragment(Point pos, int damage)
	: PlayerBullet(pos, Point(), 7)
{
	this->damage = damage;
}

JasonOverheadBulletGrenadeFragment::~JasonOverheadBulletGrenadeFragment()
{
}

int JasonOverheadBulletGrenadeFragment::GetDamage(BulletDamageModifier modifier)
{
	return 10;
}
