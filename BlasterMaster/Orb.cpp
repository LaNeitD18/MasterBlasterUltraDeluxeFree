#include "Orb.h"

Orb::Orb() {
	SetState(ORB_STATE_WALKING);
}

Orb::Orb(float x, float y) {
	SetState(ORB_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

BoundingBox Orb::GetBoundingBox()
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + ORB_BBOX_WIDTH;
	float bottom = pos.y + ORB_BBOX_HEIGHT;
		
	return BoundingBox(left, top, right, bottom);
}

void Orb::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
	}

	if (v.x > 0 && pos.x > 290) {
		pos.x = 290; v.x = -v.x;
	}
}

void Orb::Render()
{
	SetAnimationType(ORB_ANI_IDLE);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Orb::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case ORB_STATE_WALKING:
		v.x = ORB_WALKING_SPEED;
	}

}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Orb
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)