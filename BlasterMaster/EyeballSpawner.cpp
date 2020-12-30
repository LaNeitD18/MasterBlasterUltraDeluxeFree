#include "EyeballSpawner.h"
#include "Utils.h"
#include "SceneArea2Overhead.h"
#include "Game.h"
#include "CannonBullet.h"
#include "Sound.h"
#include "Player.h"
#include "LinearEyeball.h"

EyeballSpawner::EyeballSpawner() {

}

EyeballSpawner::EyeballSpawner(float x, float y) {
	pos = Point(x, y);
	this->coolDown = rand() % 1000 + 500;
}

BoundingBox EyeballSpawner::GetBoundingBox()
{
	float left = pos.x + EYEBALL_BBOX_OFFSET_LEFT;
	float top = pos.y + EYEBALL_BBOX_OFFSET_TOP;
	float right = pos.x + EYEBALL_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + EYEBALL_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void EyeballSpawner::RandomDirection()
{
	int randDirX = rand() % 2;
	if (randDirX)	this->direction.x = 1;
	else			this->direction.x = -1;

	int randDirY = rand() % 2;
	if (randDirY)	this->direction.y = 1;
	else			this->direction.y = -1;
}

void EyeballSpawner::Update()
{
	if (coolDown == 0) {
		SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
		Point playerPos = Point();
		if (scene != nullptr) {
			playerPos = scene->GetTarget()->GetPosition();
		}

		int numberOfEyeball = rand() % 6 + 4;
		for (int i = 0; i < numberOfEyeball; i++) {
			RandomDirection();
			int randX = rand() % 100 + 80;
			int randY = rand() % 100 + 80;

			Point eyePos = playerPos + Point(randX * direction.x, randY * direction.y);

			LinearEyeball* eyeball = new LinearEyeball(eyePos, direction);
			eyeball->SetManager(manager);
			manager->AddElement(eyeball);
		}
	}
	
	if(coolDown >= 0) {
		coolDown--;
	}
	else {
		coolDown = rand() % 4000 + 2500;
	}

	DebugOut(L"time %d \n", coolDown);
}

void EyeballSpawner::Render()
{
	return;
}

void EyeballSpawner::SetState(int state)
{
	GameObject::SetState(state);
}