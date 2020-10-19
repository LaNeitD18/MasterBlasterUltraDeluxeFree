#include "Orb.h"

Orb::Orb() {
	SetState(ORB_STATE_WALKING);
}

Orb::Orb(float x, float y) {
	SetState(ORB_STATE_WALKING);
	pos = Point(x, y);
}

void Orb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + ORB_BBOX_WIDTH;

	if (state == ORB_STATE_DIE)
		bottom = pos.y + ORB_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + ORB_BBOX_HEIGHT;
}

void Orb::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Orb::Render()
{
	SetAnimationType(ORB_ANI_IDLE);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Orb::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case ORB_STATE_DIE:
		pos.y += ORB_BBOX_HEIGHT - ORB_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case ORB_STATE_WALKING:
		v.x = ORB_WALKING_SPEED;
	}

}