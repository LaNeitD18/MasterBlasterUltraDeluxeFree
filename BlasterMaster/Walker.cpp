#include "Walker.h"

Walker::Walker() {
	SetState(WALKER_STATE_WALKING);
}

Walker::Walker(float x, float y) {
	SetState(WALKER_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(0.25, 0.25));
}

BoundingBox Walker::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + WALKER_BBOX_WIDTH;
	float bottom;

	if (state == WALKER_STATE_DIE)
		bottom = pos.y + WALKER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WALKER_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Walker::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Walker::Render()
{
	if (state == WALKER_STATE_DIE) {
		SetAnimationType(WALKER_ANI_DIE);
	}
	else SetAnimationType(WALKER_ANI_WALKING_RIGHT);
	//else if (v.x <= 0) SetAnimationType(Walker_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Walker::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WALKER_STATE_DIE:
		pos.y += WALKER_BBOX_HEIGHT - WALKER_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case WALKER_STATE_WALKING:
		v.x = WALKER_WALKING_SPEED;
	}

}