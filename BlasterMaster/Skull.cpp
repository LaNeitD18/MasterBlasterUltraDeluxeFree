#include "Skull.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "SkullBullet.h"
#include "Camera.h"
#include "Player.h"

Skull::Skull() {
	SetState(SKULL_STATE_INIT);
}

Skull::Skull(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetState(SKULL_STATE_INIT);
	timeToChangeState = 50;
}

BoundingBox Skull::GetBoundingBox()
{
	float left = pos.x + SKULL_BBOX_OFFSET_LEFT;
	float top = pos.y + SKULL_BBOX_OFFSET_TOP;
	float right = pos.x + SKULL_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + SKULL_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Skull::FlyVertically(Point playerPos)
{
	if (wallTop) {
		v.y = 0;
	}

	if (timeToChangeState == 0) {
		if (abs(playerPos.x - pos.x) < 3) {
			SetState(SKULL_STATE_BOMBING);
		}
	}
	
}

void Skull::FlyHorizontally(Point playerPos)
{
	if (v.x < 0 && wallLeft) {
		v.x = -v.x;
		pos.x += 2;
		direction.x = -direction.x;
	}
	else if (v.x > 0 && wallRight) {
		v.x = -v.x;
		pos.x -= 2;
		direction.x = -direction.x;
	}

	if (timeToChangeState == 0) {
		if (abs(playerPos.x - pos.x) < 3) {
			SetState(SKULL_STATE_BOMBING);
		}
	}
}

void Skull::Bombing(Point playerPos)
{
	if (currentTime == 0 && state == SKULL_STATE_BOMBING) {
		SkullBullet* bullet = new SkullBullet(pos, playerPos);
		bullet->SetManager(manager);
		manager->AddElement(bullet);

		if (previousState == SKULL_STATE_FLYING_X) {
			SetState(SKULL_STATE_FLYING_X);
			timeToChangeState = 100;
		}
		else if (previousState == SKULL_STATE_FLYING_Y) {
			SetState(SKULL_STATE_FLYING_Y);
			timeToChangeState = 100;
		}
	}
}

void Skull::Update()
{
	pos += dx();
	Enemy::Update();

	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = Point();
	if (scene != nullptr) {
		playerPos = scene->GetTarget()->GetPosition();
	}

	if (state == SKULL_STATE_FLYING_X) {
		FlyHorizontally(playerPos);
	}
	else if (state == SKULL_STATE_FLYING_Y) {
		FlyVertically(playerPos);
	}
	else if (state == SKULL_STATE_BOMBING) {
		Bombing(playerPos);
	}
	else if (state == SKULL_STATE_INIT) {
		Point distance = playerPos - pos;

		if (distance.x > 0) {
			v.x = SKULL_SPEED_FLYING_X;
			direction.x = 1;
		}
		else if (distance.x < 0) {
			v.x = -SKULL_SPEED_FLYING_X;
			direction.x = -1;
		}

		SetState(SKULL_STATE_FLYING_X);
	}

	if (!Camera::GetInstance()->GetBound().IsInsideBox(pos))
		//SetState(MINI_BULLET_STATE_EXPLODE);
		manager->RemoveElement(this);
	
	if(timeToChangeState > 0)
		timeToChangeState--;

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Skull::Render()
{
	if (state == SKULL_STATE_BOMBING) {
		SetAnimationType(SKULL_ANI_BOMB);
	}
	else {
		SetAnimationType(SKULL_ANI_FLY);
	}

	if (v.x < 0) {
		isFlipVertical = false;
	}
	else if (v.x > 0) {
		isFlipVertical = true;
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Skull::SetState(int state)
{
	GameObject::SetState(state);
	float speedX = SKULL_SPEED_FLYING_X;

	switch (state) {
	case SKULL_STATE_FLYING_X:
		previousState = SKULL_STATE_FLYING_Y;
		v.x = speedX * direction.x;
		v.y = 0;
		break;
	case SKULL_STATE_FLYING_Y:
		previousState = SKULL_STATE_FLYING_X;
		v.x = 0;
		v.y = -SKULL_SPEED_FLYING_Y;
		break;
	case SKULL_STATE_BOMBING:
		v.x = 0;
		v.y = 0;
		break;
	}
}