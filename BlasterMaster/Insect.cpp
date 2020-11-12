#include "Insect.h"

Insect::Insect() {
	SetState(INSECT_STATE_WALKING);
}

Insect::Insect(float x, float y) {
	SetState(INSECT_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

BoundingBox Insect::GetBoundingBox()
{
	float left = pos.x - INSECT_BBOX_WIDTH;
	float top = pos.y - INSECT_BBOX_HEIGHT;
	float right = pos.x + INSECT_BBOX_WIDTH;
	float bottom;

	if (state == INSECT_STATE_DIE)
		bottom = pos.y + INSECT_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + INSECT_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Insect::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Insect::Render()
{
	if (state == INSECT_STATE_DIE) {
		SetAnimationType(INSECT_ANI_DIE);
	}
	else SetAnimationType(INSECT_ANI_WALKING);
	//else if (v.x <= 0) SetAnimationType(Insect_ANI_WALKING_LEFT);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Insect::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case INSECT_STATE_DIE:
		pos.y += INSECT_BBOX_HEIGHT - INSECT_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case INSECT_STATE_WALKING:
		v.x = INSECT_WALKING_SPEED;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Insect
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)