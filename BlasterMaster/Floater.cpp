#include "Floater.h"

Floater::Floater() {
	SetState(FLOATER_STATE_FLYING);
}

Floater::Floater(float x, float y) {
	SetState(FLOATER_STATE_FLYING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

BoundingBox Floater::GetBoundingBox()
{
	float left = pos.x - FLOATER_BBOX_WIDTH;
	float top = pos.y - FLOATER_BBOX_HEIGHT;
	float right = pos.x + FLOATER_BBOX_WIDTH;
	float bottom;

	if (state == FLOATER_STATE_DIE)
		bottom = pos.y + FLOATER_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + FLOATER_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Floater::Update()
{
	pos += dx();
	Enemy::Update();

	if (v.x < 0 && wallLeft) {
		v.x = -v.x;
	}
	if (v.x > 0 && wallRight) {
		v.x = -v.x;
	}
	if (v.y < 0 && wallTop) {
		v.y = -v.y;
	}
	if (v.y > 0 && wallBot) {
		v.y = -v.y;
	}

	wallBot = wallTop = wallLeft = wallRight = false;
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
	case FLOATER_STATE_FLYING:
		v.x = -FLOATER_FLYING_SPEED_X;
		v.y = FLOATER_FLYING_SPEED_Y;
	}

}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Floater
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)