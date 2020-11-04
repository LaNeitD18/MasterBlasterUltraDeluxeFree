#include "WormPod.h"

WormPod::WormPod() {
	SetState(WORMPOD_STATE_WALKING);
}

WormPod::WormPod(float x, float y) {
	SetState(WORMPOD_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(0.25, 0.25));
}

void WormPod::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + WORMPOD_BBOX_WIDTH;

	if (state == WORMPOD_STATE_DIE)
		bottom = pos.y + WORMPOD_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WORMPOD_BBOX_HEIGHT;
}

void WormPod::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void WormPod::Render()
{
	SetAnimationType(WORMPOD_ANI_FLY);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void WormPod::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WORMPOD_STATE_DIE:
		pos.y += WORMPOD_BBOX_HEIGHT - WORMPOD_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case WORMPOD_STATE_WALKING:
		v.x = WORMPOD_WALKING_SPEED;
	}

}