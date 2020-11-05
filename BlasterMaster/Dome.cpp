#include "Dome.h"

Dome::Dome() {
	SetState(DOME_STATE_WALKING);
}

Dome::Dome(float x, float y) {
	SetState(DOME_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(0.25, 0.25));
}

void Dome::GetBoundingBox(BoundingBox& box)
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + DOME_BBOX_WIDTH;
	float bottom;

	if (state == DOME_STATE_DIE)
		bottom = pos.y + DOME_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + DOME_BBOX_HEIGHT;
	box = BoundingBox(left, top, right, bottom);
}

void Dome::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Dome::Render()
{
	SetAnimationType(DOME_ANI_TELEPORT);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Dome::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case DOME_STATE_DIE:
		pos.y += DOME_BBOX_HEIGHT - DOME_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case DOME_STATE_WALKING:
		v.x = DOME_WALKING_SPEED;
	}

}