#include "Floater.h"

Floater::Floater() {
	SetState(FLOATER_STATE_WALKING);
}

Floater::Floater(float x, float y) {
	SetState(FLOATER_STATE_WALKING);
	pos = Point(x, y);
}

void Floater::GetBoundingBox(BoundingBox& box)
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + FLOATER_BBOX_WIDTH;
	float bottom;

	if (state == FLOATER_STATE_DIE)
		bottom = pos.y + FLOATER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + FLOATER_BBOX_HEIGHT;
	box = BoundingBox(left, top, right, bottom);
}

void Floater::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Floater::Render()
{
	SetAnimationType(FLOATER_ANI_FLY);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Floater::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case FLOATER_STATE_DIE:
		pos.y += FLOATER_BBOX_HEIGHT - FLOATER_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case FLOATER_STATE_WALKING:
		v.x = FLOATER_WALKING_SPEED;
	}

}