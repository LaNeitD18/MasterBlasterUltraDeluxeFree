#include "Floater.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "MiniRedBullet.h"
#include <time.h>

Floater::Floater() {
	SetState(FLOATER_STATE_FLYING);
}

Floater::Floater(float x, float y) {
	SetState(FLOATER_STATE_FLYING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	timeToShoot = 0;
	jumpingTurn = 0;
	v.x = -FLOATER_FLYING_SPEED_X;
	v.y = FLOATER_FLYING_SPEED_Y;
}

BoundingBox Floater::GetBoundingBox()
{
	float left = pos.x + FLOATER_BBOX_OFFSET_LEFT;
	float top = pos.y + FLOATER_BBOX_OFFSET_TOP;
	float right = pos.x + FLOATER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + FLOATER_BBOX_OFFSET_BOTTOM;
		
	return BoundingBox(left, top, right, bottom);
}

void Floater::Shoot()
{
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = scene->GetTarget()->GetPosition();

	if (playerPos.y > pos.y + FLOATER_BBOX_HEIGHT) {
		MiniRedBullet* bullet = new MiniRedBullet(pos, playerPos);
		bullet->SetManager(manager);
		manager->AddElement(bullet);
		//DebugOut(L"pos %f \n", playerPos.x);
	}

	if (currentTime == 0) {
		SetState(FLOATER_STATE_FLYING);
	}
}

void Floater::Update()
{
	pos += dx();
	Enemy::Update();

	if (v.x < 0 && wallLeft) {
		v.x = -v.x;
	}
	else if (v.x > 0 && wallRight) {
		v.x = -v.x;
	}
	else if (v.y < 0 && wallTop) {
		v.y = -v.y;
	}
	else if (v.y > 0 && wallBot) {
		v.y = -v.y;
	}

	if (timeToShoot == 0) {
		if (jumpingTurn != 0) {
			SetState(FLOATER_STATE_SHOOTING);
		}
		else {
			srand(time(NULL));
			jumpingTurn = rand() % 1 + 3;
			timeToShoot = 180;
		}
	}

	if (state == FLOATER_STATE_SHOOTING) {
		Shoot();
		SetState(FLOATER_STATE_FLYING);
		jumpingTurn--;
		timeToShoot = 30;
	}
	timeToShoot--;

	wallBot = wallTop = wallLeft = wallRight = false;
}

void Floater::Render()
{
	if (state == FLOATER_STATE_FLYING)
		SetAnimationType(FLOATER_ANI_FLY);
	else if (state == FLOATER_STATE_SHOOTING)
		SetAnimationType(FLOATER_ANI_SHOOT);

	AnimatedGameObject::Render();
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Floater
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)