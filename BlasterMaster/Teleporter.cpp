#include "Teleporter.h"

Teleporter::Teleporter() {
	SetState(TELEPORTER_STATE_WALKING);
}

Teleporter::Teleporter(float x, float y) {
	SetState(TELEPORTER_STATE_WALKING);
	pos = Point(x, y);
}

void Teleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + TELEPORTER_BBOX_WIDTH;

	if (state == TELEPORTER_STATE_DIE)
		bottom = pos.y + TELEPORTER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + TELEPORTER_BBOX_HEIGHT;
}

void Teleporter::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Teleporter::Render()
{
	int ani = TELEPORTER_ANI_TELEPORT;
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	animation_set->at(ani)->Render(pos);

	//RenderBoundingBox();
}

void Teleporter::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_DIE:
		pos.y += TELEPORTER_BBOX_HEIGHT - TELEPORTER_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case TELEPORTER_STATE_WALKING:
		v.x = TELEPORTER_WALKING_SPEED;
	}

}