#include "Insect.h"

Insect::Insect() {
	SetState(INSECT_STATE_FLYING_UP);
}

Insect::Insect(float x, float y) {
	SetState(INSECT_STATE_FLYING_UP);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	direction = Point(-1, -1);
	flyUpRange = flyDownRange = 0;
}

BoundingBox Insect::GetBoundingBox()
{
	float left = pos.x + INSECT_BBOX_OFFSET_LEFT;
	float top = pos.y + INSECT_BBOX_OFFSET_TOP;
	float right = pos.x + INSECT_BBOX_OFFSET_RIGHT;
	float bottom;

	if (state == INSECT_STATE_DIE)
		bottom = pos.y + INSECT_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + INSECT_BBOX_OFFSET_BOTTOM;
	return BoundingBox(left, top, right, bottom);
}

void Insect::FlyUp() {
	flyDownRange = 0;
	if (wallTop) {
		SetState(INSECT_STATE_FALLING);
	} 
	else {
		if (flyUpRange > 32) {
			SetState(INSECT_STATE_FLYING_DOWN);
		}
	}
}

void Insect::FlyDown() {
	flyUpRange = 0;
	if (flyDownRange > 20) {
		SetState(INSECT_STATE_FLYING_UP);
	}
}

void Insect::Fall() {
	if (wallBot) {
		SetState(INSECT_STATE_FLYING_UP);
	}
}

void Insect::Update()
{
	pos += dx();
	Enemy::Update();

	if (wallLeft) {
		direction.x = 1;
	}
	else if (wallRight) {
		direction.x = -1;
	}

	if (state == INSECT_STATE_FLYING_UP) {
		flyUpRange += abs(dx().y);
		FlyUp();
	}
	else if (state == INSECT_STATE_FLYING_DOWN) {
		flyDownRange += abs(dx().y);
		FlyDown();
	}
	else if (state == INSECT_STATE_FALLING) {
		Fall();
	}
	
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Insect::Render()
{
	if (state == INSECT_STATE_DIE) {
		SetAnimationType(INSECT_ANI_DIE);
	}
	else {
		SetAnimationType(INSECT_ANI_FLYING);
		if (direction.x == 1) {
			isFlipVertical = true;
		}
		else {
			isFlipVertical = false;
		}
	}
	//else if (v.x <= 0) SetAnimationType(Insect_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Insect::SetState(int state)
{
	GameObject::SetState(state);
	float speedX = INSECT_FLYING_SPEED_X;

	switch (state)
	{
	case INSECT_STATE_DIE:
		pos.y += INSECT_BBOX_HEIGHT - INSECT_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case INSECT_STATE_FLYING_UP:
		v.x = speedX * direction.x;
		v.y = INSECT_FLYING_SPEED_Y_UP;
		break;
	case INSECT_STATE_FLYING_DOWN:
		v.x = speedX * direction.x;
		v.y = INSECT_FLYING_SPEED_Y_DOWN;
		break;
	case INSECT_STATE_FALLING:
		v.x = 0;
		v.y = 2.5;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Insect
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)