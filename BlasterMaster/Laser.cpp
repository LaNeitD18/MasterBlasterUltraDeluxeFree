#include "Laser.h"

Laser::Laser() {
	SetState(LASER_STATE_WALKING);
}

Laser::Laser(float x, float y) {
	SetState(LASER_STATE_WALKING);
	pos = Point(x, y);
}

void Laser::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + LASER_BBOX_WIDTH;

	if (state == LASER_STATE_DIE)
		bottom = pos.y + LASER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + LASER_BBOX_HEIGHT;
}

void Laser::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Laser::Render()
{
	SetAnimationType(LASER_ANI_TELEPORT);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Laser::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case LASER_STATE_DIE:
		pos.y += LASER_BBOX_HEIGHT - LASER_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case LASER_STATE_WALKING:
		v.x = LASER_WALKING_SPEED;
	}

}