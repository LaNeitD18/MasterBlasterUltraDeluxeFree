#include "MineBullet.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "Camera.h"

MineBullet::MineBullet()
{

}

MineBullet::MineBullet(Point pos, Point v)
{
	this->pos = pos;
	this->v = v;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(MINE_BULLET_ANIMATION_SET_ID));
	SetState(MINE_BULLET_STATE_NORMAL);
}

BoundingBox MineBullet::GetBoundingBox()
{
	float left = pos.x + MINE_BULLET_BBOX_OFFSET_LEFT;
	float top = pos.y + MINE_BULLET_BBOX_OFFSET_TOP;
	float right = pos.x + MINE_BULLET_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + MINE_BULLET_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void MineBullet::Update()
{
	pos += dx();
	if (v.x < 0) {
		v.x += 0.001;
	}
	else if (v.x > 0) {
		v.x -= 0.001;
	}
	v.y += 0.02;

	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	if (scene != nullptr) {
		if (scene->GetTarget()->GetBoundingBox().IsInsideBox(pos)) {
			if (wallBot || wallTop || wallLeft || wallRight) {
				v = Point();
				SetState(MINE_BULLET_STATE_EXPLODE);
			}
		}
	}
	if (currentTime == 0 && state == MINE_BULLET_STATE_EXPLODE)
		manager->RemoveElement(this);

	if (!Camera::GetInstance()->GetBound().IsInsideBox(pos))
		manager->RemoveElement(this);
}

void MineBullet::Render()
{
	if (state == MINE_BULLET_STATE_NORMAL) {
		SetAnimationType(MINE_BULLET_ANI_NORMAL);
	}
	else if (state == MINE_BULLET_STATE_EXPLODE) {
		SetAnimationType(MINE_BULLET_ANI_EXPLODE);
	}

	AnimatedGameObject::Render();
}