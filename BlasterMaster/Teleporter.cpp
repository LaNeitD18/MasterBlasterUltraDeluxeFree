#include "Teleporter.h"

Teleporter::Teleporter() {
	SetState(TELEPORTER_STATE_WALKING);
}

void Teleporter::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + TELEPORTER_BBOX_WIDTH;

	if (state == TELEPORTER_STATE_DIE)
		bottom = y + TELEPORTER_BBOX_HEIGHT_DIE;
	else
		bottom = y + TELEPORTER_BBOX_HEIGHT;
}

void Teleporter::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Koopas can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (vx < 0 && x < 0) {
		x = 0; vx = -vx;
	}

	if (vx > 0 && x > 290) {
		x = 290; vx = -vx;
	}
}

void Teleporter::Render()
{
	int ani = TELEPORTER_ANI_TELEPORT;
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Teleporter::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_DIE:
		y += TELEPORTER_BBOX_HEIGHT - TELEPORTER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case TELEPORTER_STATE_WALKING:
		vx = TELEPORTER_WALKING_SPEED;
	}

}