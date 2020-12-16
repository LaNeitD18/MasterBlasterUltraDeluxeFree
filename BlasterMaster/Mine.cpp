#include "Mine.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"
#include "MineBullet.h"
#include "time.h"
#include "Sound.h"

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

	if (isCollided || HealthPoint <= 0) {
		int numberOfBullet = rand() % 5;

		for (int i = 0; i < numberOfBullet; i++) {
			MineBullet* bullet = new MineBullet(pos, bulletV);
			bullet->SetManager(manager);
			manager->AddElement(bullet);
			bulletV.x += 0.1;
		}

		manager->RemoveElement(this);
	}

	if (currentTime == 0 ) {
		Sound::getInstance()->play("mine", false, 1);
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