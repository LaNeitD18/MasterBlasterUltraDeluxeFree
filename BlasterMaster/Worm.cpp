#include "Worm.h"

Worm::Worm() {
	SetState(WORM_STATE_WALKING);
}

Worm::Worm(float x, float y)
{
	SetState(WORM_STATE_WALKING);
	pos = Point(x, y);
}

void Worm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + WORM_BBOX_WIDTH;

	if (state == WORM_STATE_DIE)
		bottom = pos.y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WORM_BBOX_HEIGHT;
}

void Worm::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Worm::Render()
{
	if (state == WORM_STATE_DIE) {
		SetAnimationType(WORM_ANI_DIE);
	}
	else if (v.x > 0) SetAnimationType(WORM_ANI_WALKING_RIGHT);
	else if (v.x <= 0) SetAnimationType(WORM_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
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
	}

}