#include "Jumper.h"

Jumper::Jumper() {
	SetState(JUMPER_STATE_WALKING);
}

Jumper::Jumper(float x, float y) {
	SetState(JUMPER_STATE_WALKING);
	pos = Point(x, y);
}

void Jumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + JUMPER_BBOX_WIDTH;

	if (state == JUMPER_STATE_DIE)
		bottom = pos.y + JUMPER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + JUMPER_BBOX_HEIGHT;
}

void Jumper::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Jumper::Render()
{
	if (state == JUMPER_STATE_DIE) {
		SetAnimationType(JUMPER_ANI_DIE);
	}
	else if (v.x > 0) SetAnimationType(JUMPER_ANI_WALKING_RIGHT);
	//else if (v.x <= 0) SetAnimationType(JUMPER_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Jumper::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_DIE:
		pos.y += JUMPER_BBOX_HEIGHT - JUMPER_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case JUMPER_STATE_WALKING:
		v.x = JUMPER_WALKING_SPEED;
	}

}