#include "Ship.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "MiniRedBullet.h"

Ship::Ship() {
	SetState(SHIP_STATE_FLYING);
}

Ship::Ship(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetState(SHIP_STATE_FLYING);
	timeToShoot = 0;
	shootTurn = 0;
}

BoundingBox Ship::GetBoundingBox()
{
	float left = pos.x + SHIP_BBOX_OFFSET_LEFT;
	float top = pos.y + SHIP_BBOX_OFFSET_TOP;
	float right = pos.x + SHIP_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + SHIP_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Ship::Shoot()
{
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = Point();
	if (scene != nullptr) {
		playerPos = scene->GetTarget()->GetPosition();
	}

	if (playerPos.y > pos.y + (SHIP_BBOX_OFFSET_BOTTOM * 4)) {
		MiniRedBullet* bullet = new MiniRedBullet(pos, playerPos);
		bullet->SetManager(manager);
		manager->AddElement(bullet);
		//DebugOut(L"pos %f \n", playerPos.x);
	}
}

void Ship::Update()
{
	pos += dx();
	Enemy::Update();

	if (wallLeft) {
		v.x = SHIP_FLYING_SPEED;
	}
	else if (wallRight) {
		v.x = -SHIP_FLYING_SPEED;
	}

	// bắn hết lượt cuối thì reset số lượt và time, lần đầu khởi tạo thì cũng coi như vừa bắn xong lần cuối
	if (timeToShoot == 0) {
		// nếu còn lượt thì bắn, trừ lượt và đặt lại time cho viên tiếp theo
		if (shootTurn != 0) {
			Shoot();
			shootTurn--;
			timeToShoot = 30;
		}
		else {
			shootTurn = rand() % 1 + 4;
			timeToShoot = 250;
		}
	}
	timeToShoot--;

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Ship::Render()
{
	SetAnimationType(SHIP_ANI_FLY);

	if (v.x < 0) {
		isFlipVertical = false;
	}
	else if (v.x > 0) {
		isFlipVertical = true;
	}

	AnimatedGameObject::Render();
}

void Ship::SetState(int state)
{
	GameObject::SetState(state);

	switch (state) {
	case SHIP_STATE_FLYING:
		v.x = -SHIP_FLYING_SPEED;
	}
}
