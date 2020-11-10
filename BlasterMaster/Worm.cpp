#include "Worm.h"
#include "Utils.h"

Worm::Worm() {
	SetState(WORM_STATE_FALLING);
}

Worm::Worm(float x, float y)
{
	leftEdge = rightEdge = 0;
	SetState(WORM_STATE_FALLING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	isFlipVertical = true;
}

BoundingBox Worm::GetBoundingBox()
{
	float left = pos.x - WORM_BBOX_WIDTH;
	float top = pos.y - WORM_BBOX_HEIGHT;
	float right = pos.x + WORM_BBOX_WIDTH;
	float bottom;
	if (state == WORM_STATE_DIE)
		bottom = pos.y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WORM_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Worm::Fall() 
{
	if (!wallLeft)
		v.x = -WORM_FALLING_SPEED_X;
}

void Worm::Walk()
{
	if (v.x < 0 && wallLeft || v.x > 0 && wallRight) {
		v.x = -v.x;
	}
	if (v.x > 0 && pos.x > rightEdge) {
		pos.x = rightEdge; v.x = -v.x;
	}
}

void Worm::Update()
{
	pos += dx();

	if (state == WORM_STATE_FALLING) {
		Fall();
		if (wallBot) {
			SetState(WORM_STATE_WALKING);
			if(rightEdge == 0)	setRightEdge(pos.x);
		}
	}
	else if (state == WORM_STATE_WALKING) {
		Walk();
		if (!wallBot) {
			SetState(WORM_STATE_FALLING);
		}
	}
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Worm::Render()
{
	if (state == WORM_STATE_DIE) {
		SetAnimationType(WORM_ANI_DIE);
	}
	else if (state == WORM_STATE_FALLING) {
		SetAnimationType(WORM_ANI_IDLING);
	}
	else {
		SetAnimationType(WORM_ANI_WALKING_RIGHT);
		if (v.x > 0) {
			
			isFlipVertical = false;
		}
		else  isFlipVertical = true;
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

int Worm::getLeftEdge()
{
	return this->leftEdge;
}

void Worm::setLeftEdge(int left)
{
	this->leftEdge = left;
}

int Worm::getRightEdge()
{
	return this->rightEdge;
}

void Worm::setRightEdge(int right)
{
	this->rightEdge = right;
}

void Worm::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		pos.y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case WORM_STATE_WALKING:
		v.x = WORM_WALKING_SPEED;
		v.y = 0;
		break;
	case WORM_STATE_FALLING:
		v.y = WORM_FALLING_SPEED_Y;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Worm
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS