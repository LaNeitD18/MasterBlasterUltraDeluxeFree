#include "Cannon.h"
#include "CannonBullet.h"
#include "Utils.h"

Cannon::Cannon() {
	SetState(CANNON_STATE_VERTICAL);
}

Cannon::Cannon(float x, float y) {
	SetState(CANNON_STATE_VERTICAL);
	isVertical = true;
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
}

BoundingBox Cannon::GetBoundingBox()
{
	float left = pos.x + CANNON_BBOX_OFFSET_LEFT;
	float top = pos.y + CANNON_BBOX_OFFSET_TOP;
	float right = pos.x + CANNON_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + CANNON_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Cannon::ShootHorizontally()
{
	Point leftV = Point(-1, 0);
	CannonBullet* leftBullet = new CannonBullet(pos, leftV);
	leftBullet->SetManager(manager);
	manager->AddElement(leftBullet);

	Point rightV = Point(1, 0);
	CannonBullet* rightBullet = new CannonBullet(pos, rightV);
	rightBullet->SetManager(manager);
	manager->AddElement(rightBullet);
}

void Cannon::ShootVertically()
{
	Point topV = Point(0, -1);
	CannonBullet* topBullet = new CannonBullet(pos, topV);
	topBullet->SetManager(manager);
	manager->AddElement(topBullet);

	Point botV = Point(0, 1);
	CannonBullet* bottomBullet = new CannonBullet(pos, botV);
	bottomBullet->SetManager(manager);
	manager->AddElement(bottomBullet);
}

void Cannon::Update()
{
	pos += dx();

	// xai bien isVertical la vi no update ma chua setani thi da set state khac nen chi render dc moi 1 state
	if (currentTime == 0 && state == CANNON_STATE_VERTICAL && isVertical == true) {
		SetState(CANNON_STATE_HORIZONTAL);
		ShootHorizontally();
	}
	else if (currentTime == 0 && state == CANNON_STATE_HORIZONTAL && isVertical == false) {
		SetState(CANNON_STATE_VERTICAL);
		ShootVertically();
	}
}

void Cannon::Render()
{
	if (state == CANNON_STATE_VERTICAL) {
		SetAnimationType(CANNON_ANI_VERTICAL);
		isVertical = true;
	}
	else if (state == CANNON_STATE_HORIZONTAL) {
		SetAnimationType(CANNON_ANI_HORIZONTAL);
		isVertical = false;
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Cannon::SetState(int state)
{
	GameObject::SetState(state);
}
