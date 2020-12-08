#include "SkullBullet.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"

SkullBullet::SkullBullet()
{

}

SkullBullet::SkullBullet(Point pos, Point playerPos)
{
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	timeToExplode = 500;

	float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
	Point distance = (playerPos - pos);

	v.x = distance.x / module * 2;
	v.y = distance.y / module * 3;

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(SKULL_BULLET_ANIMATION_SET_ID));
	SetState(SKULL_BULLET_STATE_NORMAL);
}

BoundingBox SkullBullet::GetBoundingBox()
{
	float left = pos.x + SKULL_BULLET_BBOX_OFFSET_LEFT;
	float top = pos.y + SKULL_BULLET_BBOX_OFFSET_TOP;
	float right = pos.x + SKULL_BULLET_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + SKULL_BULLET_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void SkullBullet::Update()
{
	pos += dx();

	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		if (scene->GetTarget()->GetBoundingBox().IsInsideBox(pos)) {
			v = Point(0, 0);
			SetState(SKULL_BULLET_STATE_EXPLODE);
			//DebugOut(L"Bomb\n");
			// tai sao co luc bomb rot xuong dat nhung ko thay ani explode
		}
		else {
			if (timeToExplode == 0) {
				v.x = 0;
				SetState(SKULL_BULLET_STATE_EXPLODE);
			}
			else {
				if (wallBot) {
					v.y = 0;
				}
				else {
					v.y = 1;
				}

				if ((v.x < 0 && wallLeft) || (v.x > 0 && wallRight)) {
					v.x = -v.x;
				}
			}
		}
	}

	if (currentTime == 0 && state == SKULL_BULLET_STATE_EXPLODE)
		manager->RemoveElement(this);

	timeToExplode--;
	wallBot = wallLeft = wallRight = wallTop = false;
}

void SkullBullet::Render()
{
	if (state == SKULL_BULLET_STATE_NORMAL) {
		SetAnimationType(SKULL_BULLET_ANI_NORMAL);
	}
	else if (state == SKULL_BULLET_STATE_EXPLODE) {
		SetAnimationType(SKULL_BULLET_ANI_EXPLODE);
	}

	AnimatedGameObject::Render();
}
