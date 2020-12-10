#include "Orb.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "Camera.h"

Orb::Orb() {
	SetState(ORB_STATE_FLYING);
}

Orb::Orb(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetState(ORB_STATE_FLYING);
	initialTime = GetTickCount64();
}

BoundingBox Orb::GetBoundingBox()
{
	float left = pos.x + ORB_BBOX_OFFSET_LEFT;
	float top = pos.y + ORB_BBOX_OFFSET_TOP;
	float right = pos.x + ORB_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + ORB_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Orb::Update()
{
	pos += dx();
	Enemy::Update();

	DWORD time = GetTickCount64() - initialTime;
	DebugOut(L"time %d\n", time);

	if (time > 2500) {
		SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
		Point playerPos = Point();
		if (scene != nullptr) {
			playerPos = scene->GetTarget()->GetPosition();
		}

		float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
		Point distance = (playerPos - pos);

		v.x = distance.x / module * 0.5;
		v.y = distance.y / module * 0.5;
	}

	if (isCollided) {
		SetState(ORB_STATE_EXPLODE);
	}

	if (currentTime == 0 && state == ORB_STATE_EXPLODE) {
		manager->RemoveElement(this);
	}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Orb::Render()
{
	if (state == ORB_STATE_FLYING) {
		SetAnimationType(ORB_ANI_FLY);
	}
	else if(state == ORB_STATE_EXPLODE) {
		SetAnimationType(ORB_ANI_EXPLODE);
	}
	AnimatedGameObject::Render();
}

void Orb::SetState(int state)
{
	GameObject::SetState(state);
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Orb
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)