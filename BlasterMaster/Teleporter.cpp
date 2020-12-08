#include "Teleporter.h"

Teleporter::Teleporter() {
	SetState(TELEPORTER_STATE_TELEPORT);
}

Teleporter::Teleporter(float x, float y) {
	SetState(TELEPORTER_STATE_TELEPORT);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

BoundingBox Teleporter::GetBoundingBox()
{
	float left = pos.x + TELEPORTER_BBOX_OFFSET_LEFT;
	float top = pos.y + TELEPORTER_BBOX_OFFSET_TOP;
	float right = pos.x + TELEPORTER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + TELEPORTER_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Teleporter::Update()
{
	pos += dx();
	Enemy::Update();

}

void Teleporter::Render()
{
	if (state == TELEPORTER_STATE_TELEPORT) {
		SetAnimationType(TELEPORTER_ANI_TELEPORT);
	}
	else if (state == TELEPORTER_STATE_PROTECT) {
		SetAnimationType(TELEPORTER_ANI_PROTECT);
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Teleporter::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_TELEPORT:
		v.x = 0;
		v.y = 0;
		break;
	case TELEPORTER_STATE_PROTECT:
		v.x = 0;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Teleporter
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)