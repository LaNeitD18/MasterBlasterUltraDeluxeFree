#include "Teleporter.h"
#include "Utils.h"
#include "Game.h"
#include "SceneArea2Overhead.h"
#include "CannonBullet.h"
#include "Player.h"

Teleporter::Teleporter() {
	SetState(TELEPORTER_STATE_ATTACK);
}

Teleporter::Teleporter(float x, float y) {
	SetState(TELEPORTER_STATE_ATTACK);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	timeToTeleport = 50;
	step = 1;
	teleportTurn = 0;
}

BoundingBox Teleporter::GetBoundingBox()
{
	float left = pos.x + TELEPORTER_BBOX_OFFSET_LEFT;
	float top = pos.y + TELEPORTER_BBOX_OFFSET_TOP;
	float right = pos.x + TELEPORTER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + TELEPORTER_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Teleporter::SetDirectionX()
{
	SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		Point playerPos = scene->GetTarget()->GetPosition();
		if (pos.x > playerPos.x + PLAYER_ZONE_WIDTH) {
			direction.x = -1;
		}
		else if (pos.x < playerPos.x - PLAYER_ZONE_WIDTH) {
			direction.x = 1;
		}
		else {
			int randNumber = rand() % 2;
			if (randNumber == 0) {
				direction.x = 1;
			}
			else {
				direction.x = -1;
			}
		}
	}
}

void Teleporter::SetDirectionY()
{
	SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		Point playerPos = scene->GetTarget()->GetPosition();
		if (pos.y > playerPos.y + PLAYER_ZONE_HEIGHT) {
			direction.y = -1;
		}
		else if (pos.y < playerPos.y - PLAYER_ZONE_HEIGHT) {
			direction.y = 1;
		}
		else {
			int randNumber = rand() % 2;
			if (randNumber == 0) {
				direction.y = 1;
			}
			else {
				direction.y = -1;
			}
		}
	}
}

void Teleporter::TeleportHorizontally()
{
	if (step == 1) {
		SetDirectionX();
	}

	if (step <= 2 || step >= 5) {
		pos.x += (8 * direction.x);
		timeToTeleport = 2;
		step++;
	} 
	else {
		pos.x -= (8 * direction.x);
		timeToTeleport = 2;
		step++;
	}

	if (step == 7) {
		SetState(TELEPORTER_STATE_TELEPORT_Y);
		timeToTeleport = 50;
		step = 1;
		teleportTurn++;
		//pos.x = 350;
	}
}

void Teleporter::TeleportVertically()
{
	if (step == 1) {
		SetDirectionY();
	}

	if (step <= 2 || step >= 5) {
		pos.y += (8 * direction.y);
		timeToTeleport = 2;
		step++;
	}
	else {
		pos.y -= (8 * direction.y);
		timeToTeleport = 2;
		step++;
	}

	if (step == 7) {
		SetState(TELEPORTER_STATE_TELEPORT_X);
		timeToTeleport = 50;
		step = 1;
		teleportTurn++;
		//pos.y = 1625;
	}
}

void Teleporter::Shoot()
{
	SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		Point playerPos = scene->GetTarget()->GetPosition();
		
		float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
		Point distance = (playerPos - pos);

		Point bulletV;
		bulletV.x = distance.x / module * 1;
		bulletV.y = distance.y / module * 1.5;

		CannonBullet* bullet = new CannonBullet(pos, bulletV);
		bullet->SetManager(manager);
		manager->AddElement(bullet);
	}
}

void Teleporter::Update()
{
	Enemy::Update();

	if (timeToTeleport == 0) {
		if (state == TELEPORTER_STATE_TELEPORT_X) {
			TeleportHorizontally();
		}
		else if (state == TELEPORTER_STATE_TELEPORT_Y) {
			TeleportVertically();
		}
	}

	if (currentTime == 0) {
		if (state == TELEPORTER_STATE_ATTACK) {
			SetState(TELEPORTER_STATE_TELEPORT_X);
		}
		else if (state == TELEPORTER_STATE_PROTECT) {
			SetState(TELEPORTER_STATE_ATTACK);
		}
	}

	if ((teleportTurn == 4 || teleportTurn == 8) && timeToTeleport == 50) {
		Shoot();
	}

	if (teleportTurn == 8) {
		SetState(TELEPORTER_STATE_PROTECT);
	}

	if(timeToTeleport > 0)
		timeToTeleport--;
}

void Teleporter::Render()
{
	if (state == TELEPORTER_STATE_TELEPORT_X || state == TELEPORTER_STATE_TELEPORT_Y) {
		SetAnimationType(TELEPORTER_ANI_TELEPORT);
	}
	else if (state == TELEPORTER_STATE_PROTECT) {
		SetAnimationType(TELEPORTER_ANI_PROTECT);
	}
	else if (state == TELEPORTER_STATE_ATTACK) {
		SetAnimationType(TELEPORTER_ANI_ATTACK);
	}

	AnimatedGameObject::Render();
}

void Teleporter::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case TELEPORTER_STATE_ATTACK:
		teleportTurn = 0;
		break;
	case TELEPORTER_STATE_PROTECT:
		teleportTurn = 0;
		break;
	}
}