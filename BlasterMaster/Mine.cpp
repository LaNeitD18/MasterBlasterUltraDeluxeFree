#include "Mine.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"
#include "MineBullet.h"
#include "time.h"

Mine::Mine() {
	
}

Mine::Mine(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	//SetState(SKULL_STATE_INIT);
}

BoundingBox Mine::GetBoundingBox()
{
	float left = pos.x + MINE_BBOX_OFFSET_LEFT;
	float top = pos.y + MINE_BBOX_OFFSET_TOP;
	float right = pos.x + MINE_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + MINE_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Mine::Update()
{
	pos += dx();
	Enemy::Update();
	Point bulletV = Point(-0.2, -1);

	if (isCollided) {
		srand(time(NULL));
		int numberOfBullet = rand() % 5;

		for (int i = 0; i < numberOfBullet; i++) {
			MineBullet* bullet = new MineBullet(pos, bulletV);
			bullet->SetManager(manager);
			manager->AddElement(bullet);
			bulletV.x += 0.1;
		}

		manager->RemoveElement(this);
	}
}

void Mine::Render()
{
	SetAnimationType(MINE_ANI_NORMAL);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Mine::SetState(int state)
{
	GameObject::SetState(state);
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Mine
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)