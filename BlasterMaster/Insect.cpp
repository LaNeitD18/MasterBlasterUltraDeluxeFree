#include "Insect.h"

Insect::Insect() {
	SetState(INSECT_STATE_WALKING);
}

Insect::Insect(float x, float y) {
	SetState(INSECT_STATE_WALKING);
	pos = Point(x, y);
}

void Insect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + INSECT_BBOX_WIDTH;

	if (state == INSECT_STATE_DIE)
		bottom = pos.y + INSECT_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + INSECT_BBOX_HEIGHT;
}

void Insect::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Insect::Render()
{
	if (state == INSECT_STATE_DIE) {
		SetAnimationType(INSECT_ANI_DIE);
	}
	else SetAnimationType(INSECT_ANI_WALKING_RIGHT);
	//else if (v.x <= 0) SetAnimationType(Insect_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Insect::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case INSECT_STATE_DIE:
		pos.y += INSECT_BBOX_HEIGHT - INSECT_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case INSECT_STATE_WALKING:
		v.x = INSECT_WALKING_SPEED;
	}

}