#include "Worm.h"

Worm::Worm() {
	SetState(WORM_STATE_WALKING);
}

void Worm::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + WORM_BBOX_WIDTH;

	if (state == WORM_STATE_DIE)
		bottom = y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = y + WORM_BBOX_HEIGHT;
}

void Worm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

void Worm::Render()
{
	int ani = WORM_ANI_WALKING_LEFT;
	if (state == WORM_STATE_DIE) {
		ani = WORM_ANI_DIE;
	}
	else if (vx > 0) ani = WORM_ANI_WALKING_RIGHT;
	else if (vx <= 0) ani = WORM_ANI_WALKING_LEFT;

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void Worm::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case WORM_STATE_WALKING:
		vx = WORM_WALKING_SPEED;
	}

}