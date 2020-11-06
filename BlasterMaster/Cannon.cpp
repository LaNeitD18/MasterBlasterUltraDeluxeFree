#include "Cannon.h"

Cannon::Cannon() {
	SetState(CANNON_STATE_WALKING);
}

Cannon::Cannon(float x, float y) {
	SetState(CANNON_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(0.25, 0.25));
}

BoundingBox Cannon::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + CANNON_BBOX_WIDTH;
	float bottom;

	if (state == CANNON_STATE_DIE)
		bottom = pos.y + CANNON_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + CANNON_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Cannon::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Cannon::Render()
{
	SetAnimationType(CANNON_ANI_TELEPORT);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Cannon::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case CANNON_STATE_DIE:
		pos.y += CANNON_BBOX_HEIGHT - CANNON_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case CANNON_STATE_WALKING:
		v.x = CANNON_WALKING_SPEED;
	}

}