#include "Jumper.h"

Jumper::Jumper() {
	SetState(JUMPER_STATE_WALKING);
}

void Jumper::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + JUMPER_BBOX_WIDTH;

	if (state == JUMPER_STATE_DIE)
		bottom = y + JUMPER_BBOX_HEIGHT_DIE;
	else
		bottom = y + JUMPER_BBOX_HEIGHT;
}

void Jumper::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void Jumper::Render()
{
	int ani = JUMPER_ANI_WALKING_LEFT;
	if (state == JUMPER_STATE_DIE) {
		ani = JUMPER_ANI_DIE;
	}
	else if (vx > 0) ani = JUMPER_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = JUMPER_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Jumper::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_DIE:
		y += JUMPER_BBOX_HEIGHT - JUMPER_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case JUMPER_STATE_WALKING:
		vx = JUMPER_WALKING_SPEED;
	}

}