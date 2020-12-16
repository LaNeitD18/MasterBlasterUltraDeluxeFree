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
	/*
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
	//*/
	isFlipVertical = true;

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

	rotation = atan2(v.y, v.x);

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
	case 7:
		SetAnimationType(BULLET_ANI_GRENADE_FRAG);
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
	Managed<Bullet>::manager->RemoveElement(this);
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

JasonSideviewBullet::JasonSideviewBullet(Point pos, Point v)
	: TimedPlayerBullet(pos, v, BULLET_ANI_ORB_SMALL)
{
	TTL = JASON_SIDEVIEW_BULLET_TIME_TO_LIVE;
}

JasonSideviewBullet::~JasonSideviewBullet()
{
	Managed<Bullet>::manager->RemoveElement(this);
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

JasonOverheadBulletNorm::JasonOverheadBulletNorm(Point pos, Point v, float power) 
	: TimedPlayerBullet(pos, v, BULLET_ANI_ROUND_FIREBALL)
{
	TTL = JASON_OVERHEAD_BULLET_NORM_TIME_TO_LIVE_RANGE_MIN * (1 - power)
		+ JASON_OVERHEAD_BULLET_NORM_TIME_TO_LIVE_RANGE_MAX * power;
	damage = JASON_OVERHEAD_BULLET_NORM_DAMAGE_RANGE_MIN * (1 - power)
		+ JASON_OVERHEAD_BULLET_NORM_DAMAGE_RANGE_MAX * power;
}

JasonOverheadBulletNorm::~JasonOverheadBulletNorm()
{
	Managed<Bullet>::manager->RemoveElement(this);
}

int JasonOverheadBulletNorm::GetDamage(BulletDamageModifier modifier)
{
	return damage;
}

JasonOverheadBulletGrenade::JasonOverheadBulletGrenade
	(Point pos, Point v, float power)
	: TimedPlayerBullet(pos, v, BULLET_ANI_GRENADE)
{
	TTL = JASON_OVERHEAD_GRENADE_TIME_TO_LIVE;
	this->power = power;
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
	return BoundingBox(pos.x, pos.y, pos.x, pos.y);
}

void JasonOverheadBulletGrenade::Update()
{
	TimedPlayerBullet::Update();
	if (state & BULLET_STATE_EXPLODE) {
		Managed<Bullet>::manager->RemoveElement(this);
		Managed<GameObject>::manager->RemoveElement(this);

		JasonOverheadBulletGrenadeFragment* frag =
			new JasonOverheadBulletGrenadeFragment(pos, power);
		Managed<GameObject>::manager->AddElement(frag);
		frag->SetManager(Managed<GameObject>::manager);

		frag = new JasonOverheadBulletGrenadeFragment(pos, power);
		Managed<GameObject>::manager->AddElement(frag);
		frag->SetManager(Managed<GameObject>::manager);

		return;
	}
	rotation = 0;
}

TimedPlayerBullet::~TimedPlayerBullet()
{
}

void TimedPlayerBullet::Update()
{
	TTL--;
	if (TTL <= 0) {
		SetState(state | BULLET_STATE_EXPLODE);
	}

	PlayerBullet::Update();
}

JasonOverheadBulletGrenadeFragment::JasonOverheadBulletGrenadeFragment
	(Point pos, float power)
	: TimedPlayerBullet(pos, Point(), BULLET_ANI_GRENADE_FRAG)
{
	initialPos = pos;
	TTL = JASON_OVERHEAD_GRENADE_FRAGMENT_TTL_MIN * (1 - power)
		+ JASON_OVERHEAD_GRENADE_FRAGMENT_TTL_MAX * power;
	this->damage = 1;
}

JasonOverheadBulletGrenadeFragment::~JasonOverheadBulletGrenadeFragment()
{
}

int JasonOverheadBulletGrenadeFragment::GetDamage(BulletDamageModifier modifier)
{
	return damage;
}

void JasonOverheadBulletGrenadeFragment::Update()
{
	TimedPlayerBullet::Update();
	int deltaX = rand() % 21 - 10;
	int deltaY = rand() % 21 - 10;
	pos = initialPos + Point(deltaX, deltaY);

	if (state & BULLET_STATE_EXPLODE) {
		manager->RemoveElement(this);
	}
}
