#include "Bullet.h"
#include "Utils.h"
#include "Camera.h"
#include "GameGlobal.h"
#include "Sound.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define JASONO_BULLET_GRENADE_DAMAGE 1

static D3DCOLOR crushBeamColor[6] = { /*D3DCOLOR_ARGB(255,255,255,255),*/D3DCOLOR_ARGB(255,0,255,255),/*D3DCOLOR_ARGB(255,255,255,255),*/D3DCOLOR_ARGB(255,255,0,255),/*D3DCOLOR_ARGB(255,255,255,255),*/ D3DCOLOR_ARGB(255,0,255,255) };

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
	if (state & BULLET_STATE_EXPLODE)
		return BoundingBox(pos.x, pos.y, pos.x, pos.y);
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
			pos.x - BULLET_OFFSET_DOWN,
			pos.y + BULLET_OFFSET_LEFT,
			pos.x - BULLET_OFFSET_UP,
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

	if (v.length() != 0)
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
	case 6:
		SetAnimationType(BULLET_ANI_ROCKET);
		break;
	case 8:
		SetAnimationType(BULLET_ANI_GRENADE_FRAG);
		break;
	case 9:
		SetAnimationType(BULLET_ANI_BOSS);
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

void SophiaBullet::SetAnimationType(int ani)
{
	Animation* trg = animationSet->at(ani);
	if (currentAnimation != trg && ani == BULLET_ANI_EXPLODE)
	{
		Sound::getInstance()->play("sophia_bullet_explosion", false, 1);
	}
	PlayerBullet::SetAnimationType(ani);
}

int SophiaBullet::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
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

void SophiaBullet::Render()
{
	PlayerBullet::Render();
	if (GameGlobal::GetCrusherBeam() == true) {
		drawArguments.SetColor(crushBeamColor[rand() % 3]);
	}
}

JasonSideviewBullet::JasonSideviewBullet(Point pos, Point v)
	: TimedPlayerBullet(pos, v, BULLET_ANI_ORB_SMALL)
{
	TTL = JASON_SIDEVIEW_BULLET_TIME_TO_LIVE;
}

JasonSideviewBullet::~JasonSideviewBullet()
{
	if (Managed<Bullet>::manager != NULL) {
		Managed<Bullet>::manager->RemoveElement(this);
	}
}

void JasonSideviewBullet::SetAnimationType(int ani)
{
	Animation* trg = animationSet->at(ani);
	if (currentAnimation != trg && ani == BULLET_ANI_EXPLODE)
	{
		Sound::getInstance()->play("sophia_bullet_explosion", false, 1);
	}
	PlayerBullet::SetAnimationType(ani);
}

int JasonSideviewBullet::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
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
	this->power = power;
	sin_NAK = rand() % JASON_OVERHEAD_BULLET_NORM_SIN_CYCLE;
	phase = 0;
	switch (damage)
	{
	case 5:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
		break;
	case 6:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 255, 192));
		break;
	case 7:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 192, 255));
		break;
	case 8:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 192, 127));
		break;
	case 9:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 127, 255));
		break;
	case 10:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 255, 127));
		break;
	case 11:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 192, 127));
		break;
	case 12:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 127, 192));
		break;
	case 13:
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 127, 127));
		break;
	default:
		break;
	}
}

JasonOverheadBulletNorm::~JasonOverheadBulletNorm()
{
	if (Managed<Bullet>::manager != NULL)
		Managed<Bullet>::manager->RemoveElement(this);
}

void JasonOverheadBulletNorm::SetAnimationType(int ani)
{
	Animation* trg = animationSet->at(ani);
	if (currentAnimation != trg && ani == BULLET_ANI_EXPLODE)
	{
		Sound::getInstance()->play("sophia_bullet_explosion", false, 1);
	}
	PlayerBullet::SetAnimationType(ani);
}

int JasonOverheadBulletNorm::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	return damage;
}

void JasonOverheadBulletNorm::Update()
{
	phase++;
	TimedPlayerBullet::Update();

	// Sin bullet
	if (power >= JASON_OVERHEAD_BULLET_NORM_SIN_THRESHOLD)
	{
		double targetAmp = sin(phase * JASON_OVERHEAD_BULLET_NORM_SIN_OMEGA) * JASON_OVERHEAD_BULLET_NORM_SIN_WIDTH;
		double targetPrevAmp = 0;
		if (phase != 0)
			targetPrevAmp = sin((phase - 1) * JASON_OVERHEAD_BULLET_NORM_SIN_OMEGA) * JASON_OVERHEAD_BULLET_NORM_SIN_WIDTH;
		float delta = targetAmp - targetPrevAmp;
		if (sin_NAK > 0 && rand() % 3 == 0) {
			sin_NAK--;
			phase--;
			delta = 0;
		}
		switch (dir)
		{
		case BULLET_DIR_LEFT:
			v.y = delta;
			break;
		case BULLET_DIR_UP:
			v.x = -delta;
			break;
		case BULLET_DIR_RIGHT:
			v.y = -delta;
			break;
		case BULLET_DIR_DOWN:
			v.x = delta;
			break;
		default:
			DebugOut(L"Unknown Bullet Direction.\n");
			break;
		}
	}

	if (state & BULLET_STATE_EXPLODE)
		drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 255, 255));
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
	if (Managed<Bullet>::manager != NULL)
		Managed<Bullet>::manager->RemoveElement(this);
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
		Managed<GameObject>::manager->RemoveElement(this);

		JasonOverheadBulletGrenadeFragment* frag =
			new JasonOverheadBulletGrenadeFragment(pos, power);
		Managed<GameObject>::manager->AddElement(frag);
		frag->SetManager(Managed<GameObject>::manager);

		frag = new JasonOverheadBulletGrenadeFragment(pos, 
			-JASON_OVERHEAD_GRENADE_FRAGMENT_TTL_MIN / float(JASON_OVERHEAD_GRENADE_FRAGMENT_TTL_MAX - JASON_OVERHEAD_GRENADE_FRAGMENT_TTL_MIN));
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
	this->damage = JASONO_BULLET_GRENADE_DAMAGE;
}

JasonOverheadBulletGrenadeFragment::~JasonOverheadBulletGrenadeFragment()
{
}

int JasonOverheadBulletGrenadeFragment::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	return damage;
}

void JasonOverheadBulletGrenadeFragment::Update()
{
	TimedPlayerBullet::Update();
	int deltaX = rand() % 21 - 10;
	int deltaY = rand() % 21 - 10;
	pos = initialPos + Point(deltaX, deltaY);

	if (TTL <= 0) {
		manager->RemoveElement(this);
	}
}

// Thunder Break
ThunderBullet::ThunderBullet(Point pos, int numberOfThunder, int dirX, D3DCOLOR color) : PlayerBullet(pos, Point(), 0)
{
	this->pos = pos;
	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(131));
	this->numberOfThunder = numberOfThunder + 1;
	isCreateAnotherThunder = false;
	this->dirX = dirX;
	
	if (this->numberOfThunder == 1) {
		SetRandomColor();
	}
	else {
		this->drawArguments.SetColor(color);
	}
	
	int randFlip = rand() % 2;
	if (randFlip == 0) {
		isFlipVertical = false;
	}
	else {
		isFlipVertical = true;
	}

	int aniType = rand() % 6;
	SetAnimationType(aniType);
}

BoundingBox ThunderBullet::GetBoundingBox()
{
	float left = pos.x + THUNDER_BBOX_OFFSET_LEFT;
	float top = pos.y + THUNDER_BBOX_OFFSET_TOP;
	float right = pos.x + THUNDER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + THUNDER_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void ThunderBullet::SetRandomColor()
{
	int randColor = rand() % 3;
	switch (randColor)
	{
	case 0:
		this->drawArguments.SetColor(D3DCOLOR_ARGB(255, 255, 100, 100));
		break;
	case 1:
		this->drawArguments.SetColor(D3DCOLOR_ARGB(255, 100, 255, 100));
		break;
	case 2:
		this->drawArguments.SetColor(D3DCOLOR_ARGB(255, 100, 100, 255));
		break;
	default:
		break;
	}
}

void ThunderBullet::Update()
{
	if (numberOfThunder <= 2 && currentTime == TIME_TO_CREATE_ANOTHER_THUNDER && !isCreateAnotherThunder) {
		SetRandomColor();
		Point thunderPos = this->pos + Point(4*dirX, 32);
		ThunderBullet* bullet = new ThunderBullet(thunderPos, numberOfThunder, dirX, drawArguments.GetColor());
		bullet->SetManager(manager);
		manager->AddElement(bullet);
		isCreateAnotherThunder = true;
	}
		
	if (this->currentTime == TIME_TO_REMOVE) {
		manager->RemoveElement(this);
	}
}

void ThunderBullet::Render()
{
	AnimatedGameObject::Render();
}

int ThunderBullet::GetDamage(BulletDamageModifier modifier)
{
	/*
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	//*/
	if (currentTime == 0)
		return THUNDER_BULLET_DAMAGE;
	else return 0;
}

// Multiwarhead Missile
MultiwarheadMissile::MultiwarheadMissile(Point pos, int dirX, int index) : RocketBullet(pos, Point())
{
	this->pos = pos;
	this->dirX = dirX;
	this->index = index;
	
	switch (index) {
	case 1:
		this->v = Point(MULTIWARHEAD_INITIAL_SPEED_X * dirX, MULTIWARHEAD_INITIAL_SPEED_Y * 0);
		break;
	case 2:
		this->v = Point(MULTIWARHEAD_INITIAL_SPEED_X * dirX, -MULTIWARHEAD_INITIAL_SPEED_Y);
		break;
	case 3:
		this->v = Point(MULTIWARHEAD_INITIAL_SPEED_X * dirX, MULTIWARHEAD_INITIAL_SPEED_Y);
		break;
	default:
		break;
	}

	//SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(MULTIWARHEAD_ANISET_ID));
}

BoundingBox MultiwarheadMissile::GetBoundingBox()
{
	float left = pos.x + MULTIWARHEAD_BBOX_OFFSET_LEFT;
	float top = pos.y + MULTIWARHEAD_BBOX_OFFSET_TOP;
	float right = pos.x + MULTIWARHEAD_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + MULTIWARHEAD_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void MultiwarheadMissile::Update()
{
	pos += dx();

	switch (index) {
	case 2:
		v.y += MULTIWARHEAD_ACCELERATION;
		break;
	case 3:
		v.y -= MULTIWARHEAD_ACCELERATION;
		break;
	default:
		break;
	}

	if (!Camera::GetInstance()->GetBound().IsInsideBox(pos))
		manager->RemoveElement(this);
}

void MultiwarheadMissile::Render()
{
	if (v.x > 0) {
		isFlipVertical = true;
	}
	else {
		isFlipVertical = false;
	}

	AnimatedGameObject::Render();
}

int MultiwarheadMissile::GetDamage(BulletDamageModifier modifier)
{
	/*
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	//*/
	if (currentTime == 0)
		return MULTIWARHEAD_BULLET_DAMAGE;
	else return 0;
}

HomingBullet::HomingBullet(Point pos, Point v) : RocketBullet(pos, v)
{
	isTargetAvailable = false;
	target = proposedTarget = NULL;
}

HomingBullet::~HomingBullet()
{
}

int RocketBullet::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	return ROCKET_BULLET_DAMAGE;
}

void HomingBullet::Update()
{
	RocketBullet::Update();
	Point targetLocation;
	if (!isTargetAvailable)
	{
		if (proposedTarget == NULL)
		{
			// There are no targets, proposed or not
			return;
		}
		else
			target = proposedTarget;
	}
	targetLocation = target->GetPosition();
	isTargetAvailable = false;

	Point targetV = targetLocation - pos;
	float targetRotation = atan2(targetV.y, targetV.x);

	float leftRotation = targetRotation - rotation;
	if (leftRotation < 0) leftRotation += M_PI * 2;
	if (leftRotation < M_PI)
	{
		rotation += ROCKET_ROTATIOn_SPEED;
		if (rotation > M_PI * 2) rotation -= M_PI * 2;
	}
	else
	{
		rotation -= ROCKET_ROTATIOn_SPEED;
		if (rotation < 0) rotation += M_PI * 2;
	}

	v.x = cos(rotation) * ROCKET_SPEED;
	v.y = sin(rotation) * ROCKET_SPEED;
}

RocketBullet::RocketBullet(Point pos, Point v) : PlayerBullet(pos, v, BULLET_ANI_ROCKET)
{
}

RocketBullet::~RocketBullet()
{
}

BossBullet::BossBullet(Point pos, Point v) : EnemyBullet(pos, v, BULLET_ANI_BOSS)
{
}

BossBullet::~BossBullet()
{
}

int BossBullet::GetDamage(BulletDamageModifier modifier)
{
	if (state & BULLET_STATE_EXPLODE)
		return 0;
	return 10;
}
