#include "Mine.h"

Mine::Mine() {
	SetState(MINE_STATE_WALKING);
}

Mine::Mine(float x, float y) {
	SetState(MINE_STATE_WALKING);
	pos = Point(x, y);
}

void Mine::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = pos.x;
	top = pos.y;
	right = pos.x + MINE_BBOX_WIDTH;

	if (state == MINE_STATE_DIE)
		bottom = pos.y + MINE_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + MINE_BBOX_HEIGHT;
}

void Mine::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Mine::Render()
{
	SetAnimationType(MINE_ANI_TELEPORT);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Mine::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case MINE_STATE_DIE:
		pos.y += MINE_BBOX_HEIGHT - MINE_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case MINE_STATE_WALKING:
		v.x = MINE_WALKING_SPEED;
	}

}