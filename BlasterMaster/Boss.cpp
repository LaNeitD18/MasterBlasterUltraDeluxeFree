#include "Boss.h"
#include "Utils.h"
#include "Game.h"
#include "Sound.h"

Boss::Boss() {
}

Boss::Boss(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	//SetState();
}

BoundingBox Boss::GetBoundingBox()
{
	float left = pos.x + BODY_BBOX_OFFSET_LEFT;
	float top = pos.y + BODY_BBOX_OFFSET_TOP;
	float right = pos.x + BODY_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + BODY_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Boss::Update()
{
	pos += dx();
	Enemy::Update();

	
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Boss::Render()
{
	SetAnimationType(0);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Boss::SetState(int state)
{
	GameObject::SetState(state);

}

void BossArm::Update()
{
	Enemy::Update();
	v = (targetLocation - pos) * 0.5f;
	HealthPoint = MAXINT / 2;
}
