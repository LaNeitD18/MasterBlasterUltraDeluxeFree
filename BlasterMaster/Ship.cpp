#include "Ship.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"

Ship::Ship() {
	SetState(SHIP_STATE_FLYING);
}

Ship::Ship(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetState(SHIP_STATE_FLYING);
}

BoundingBox Ship::GetBoundingBox()
{
	float left = pos.x + SHIP_BBOX_OFFSET_LEFT;
	float top = pos.y + SHIP_BBOX_OFFSET_TOP;
	float right = pos.x + SHIP_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + SHIP_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Ship::Update()
{
	pos += dx();
	Enemy::Update();

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Ship::Render()
{
	SetAnimationType(SHIP_ANI_FLY);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Ship::SetState(int state)
{
	GameObject::SetState(state);
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Ship
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
