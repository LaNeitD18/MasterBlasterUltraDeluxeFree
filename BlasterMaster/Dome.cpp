#include "Dome.h"

Dome::Dome() {
	SetState(DOME_STATE_WALKING);
}

Dome::Dome(float x, float y) {
	SetState(DOME_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH, DOME_BBOX_HEIGHT));
}

BoundingBox Dome::GetBoundingBox()
{
	float left = pos.x - DOME_BBOX_WIDTH;
	float top = pos.y - DOME_BBOX_HEIGHT;
	float right = pos.x + DOME_BBOX_WIDTH;
	float bottom;

	if (state == DOME_STATE_DIE)
		bottom = pos.y + DOME_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + DOME_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Dome::Update()
{
	pos += dx();

	if (v.x < 0 && wallLeft) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN);
		previousVelocity.x = v.x;
		v.x = 0;
		v.y = -previousVelocity.y;
	}
	if (v.x > 0 && wallRight) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 3);
		previousVelocity.x = v.x;
		v.x = 0;
		v.y = -previousVelocity.y;
	}
	if (v.y < 0 && wallTop) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN*2);
		previousVelocity.y = v.y;
		v.y = 0;
		v.x = -previousVelocity.x;
	}
	if (v.y > 0 && wallBot) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN);
		previousVelocity.y = v.y;
		v.y = 0;
		v.x = -previousVelocity.x;
	}
	//2968
}

void Dome::Render()
{
	SetAnimationType(DOME_ANI_WALKING);
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
		v.x = -DOME_WALKING_SPEED_X;

		previousVelocity.x = v.x;
		previousVelocity.y = DOME_WALKING_SPEED_Y;
	}

}