#include "Eyeball.h"
#include "Utils.h"
#include "SceneArea2Overhead.h"
#include "Game.h"
#include "CannonBullet.h"
#include "Sound.h"
#include "Player.h"

Eyeball::Eyeball() {
	SetState(EYEBALL_STATE_FLYING);
}

Eyeball::Eyeball(float x, float y) {
	SetState(EYEBALL_STATE_IDLE);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	timeToStart = 100;
}

BoundingBox Eyeball::GetBoundingBox()
{
	float left = pos.x + EYEBALL_BBOX_OFFSET_LEFT;
	float top = pos.y + EYEBALL_BBOX_OFFSET_TOP;
	float right = pos.x + EYEBALL_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + EYEBALL_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Eyeball::Shoot()
{
	SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		Point playerPos = scene->GetTarget()->GetPosition();

		float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
		Point distance = (playerPos - pos);

		Point bulletV;
		bulletV.x = distance.x / module;
		bulletV.y = distance.y / module;

		CannonBullet* bullet = new CannonBullet(pos, bulletV);
		bullet->SetManager(manager);
		manager->AddElement(bullet);
	}
}

void Eyeball::Fly()
{
	v.y -= 0.005 * direction.y;

	if ((v.x < 0 && wallLeft) || (v.x > 0 && wallRight)) {
		v.x = -v.x;
		direction.x = -direction.x;
	}

	if ((v.y < 0 && wallTop) || (v.y > 0 && wallBot)) {
		v.y = -v.y;
		direction.y = -direction.y;
	}

	if (timeToFly == 0) {
		SetState(EYEBALL_STATE_STOP_FLYING);
	}
}

void Eyeball::Update()
{
	pos += dx();
	Enemy::Update();

	if (currentTime == 0) {
		if (state == EYEBALL_STATE_START_FLYING) {
			Sound::getInstance()->play("teleporter_shoot", false, 1);
			SetState(EYEBALL_STATE_FLYING);
			Shoot();
		}
		else if (state == EYEBALL_STATE_STOP_FLYING) {
			SetState(EYEBALL_STATE_IDLE);
		}
	}

	if (timeToStart == 0) {
		SetState(EYEBALL_STATE_START_FLYING);
		timeToStart = -1;
	}

	if (state == EYEBALL_STATE_FLYING) {
		Fly();
	}
	
	if (timeToStart > 0) {
		timeToStart--;
	}

	if (timeToFly > 0) {
		timeToFly--;
	}

	wallBot = wallTop = wallLeft = wallRight = false;
}

void Eyeball::Render()
{
	if (state == EYEBALL_STATE_IDLE || state == EYEBALL_STATE_FLYING) {
		SetAnimationType(EYEBALL_ANI_OPEN_EYE);
	}
	else if (state == EYEBALL_STATE_START_FLYING || state == EYEBALL_STATE_STOP_FLYING) {
		SetAnimationType(EYEBALL_ANI_CLOSE_EYE);
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Eyeball::SetState(int state)
{
	GameObject::SetState(state);
	int randNumX = rand() % 4 + 2;
	int randNumY = rand() % 4 + 2;

	int randNumber = rand() % 2;
	if (randNumber == 0) { direction.x = 1; }
	else { direction.x = -1; }

	randNumber = rand() % 2;
	if (randNumber == 0) { direction.y = 1; }
	else { direction.y = -1; }

	switch (state)
	{
	case EYEBALL_STATE_FLYING:
		timeToFly = 120;
		v.x = (float)direction.x / randNumX;
		v.y = (float)direction.y / randNumY;
		break;
	case EYEBALL_STATE_IDLE:
		timeToStart = 100;
		v = Point();
		break;
	default:
		break;
	}
}