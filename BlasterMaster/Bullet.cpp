#include "Bullet.h"
#include "Utils.h"
#include "Camera.h"
#define _USE_MATH_DEFINES
#include <math.h>

Bullet::Bullet()
{
}

Bullet::Bullet(Point pos, Point v, int level)
{
	this->pos = pos;
	this->v = v;

	state = 0;
	this->level = level;
}

Bullet::~Bullet()
{
}

BoundingBox Bullet::GetBoundingBox()
{
	if (v.y == 0)
		return BoundingBox(
			pos.x + BULLET_OFFSET_LEFT, 
			pos.y + BULLET_OFFSET_UP,
			pos.x + BULLET_OFFSET_RIGHT, 
			pos.y + BULLET_OFFSET_DOWN);
	else
		return BoundingBox(
			pos.x + BULLET_OFFSET_UP,
			pos.y + BULLET_OFFSET_LEFT,
			pos.x + BULLET_OFFSET_DOWN,
			pos.y + BULLET_OFFSET_RIGHT);
}

void Bullet::Update()
{
	if (!(state & BULLET_STATE_EXPLODE))
		pos += dx();

	if (v.y < 0) {
		rotation = M_PI_2;
		isFlipVertical = false;
	}
	else if (v.x > 0)
		isFlipVertical = true;
	else
		isFlipVertical = false;

	if (!Camera::GetInstance()->GetBound().IsInsideBox(GetBoundingBox().GetCenter()))
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

	switch (level)
	{
	case 1:
		SetAnimationType(BULLET_ANI_NORM1);
		break;
	case 2:
		SetAnimationType(BULLET_ANI_NORM2);
		break;
	default:
		//SetAnimationType(BULLET_ANI_NORM1);
		DEBUG(throw 1);
		DebugOut(L"Unknown bullet level: %d", level);
		break;
	}
}

int Bullet::GetLevel() { return level; }


#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Bullet
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS