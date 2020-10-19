#include "Eye.h"

Eye::Eye() {
	SetState(EYE_STATE_WALKING);
}

Eye::Eye(float x, float y) {
	SetState(EYE_STATE_WALKING);
	pos = Point(x, y);
}

void Eye::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + EYE_BBOX_WIDTH;

	if (state == EYE_STATE_DIE)
		bottom = pos.y + EYE_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + EYE_BBOX_HEIGHT;
}

void Eye::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Eye::Render()
{
	SetAnimationType(EYE_ANI_TELEPORT);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Eye::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case EYE_STATE_DIE:
		pos.y += EYE_BBOX_HEIGHT - EYE_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case EYE_STATE_WALKING:
		v.x = EYE_WALKING_SPEED;
	}

}