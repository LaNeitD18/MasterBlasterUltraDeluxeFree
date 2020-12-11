#include "CannonBullet.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"

CannonBullet::CannonBullet()
{

}

CannonBullet::CannonBullet(Point pos, Point v)
{
	this->pos = pos;
	this->v = v;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(CANNON_BULLET_ANIMATION_SET_ID));
	SetState(CANNON_BULLET_STATE_NORMAL);
}

BoundingBox CannonBullet::GetBoundingBox()
{
	float left = pos.x + CANNON_BULLET_BBOX_OFFSET_LEFT;
	float top = pos.y + CANNON_BULLET_BBOX_OFFSET_TOP;
	float right = pos.x + CANNON_BULLET_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + CANNON_BULLET_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void CannonBullet::Update()
{
	pos += dx();

	if (wallBot || wallTop || wallLeft || wallRight) {
		v = Point();
		SetState(CANNON_BULLET_STATE_EXPLODE);
	}

	if (currentTime == 0 && state == CANNON_BULLET_STATE_EXPLODE) {
		this->manager->RemoveElement(this);
	}
}

void CannonBullet::Render()
{
	if (state == CANNON_BULLET_STATE_NORMAL) {
		SetAnimationType(CANNON_BULLET_ANI_NORMAL);
	}
	else if (state == CANNON_BULLET_STATE_EXPLODE) {
		SetAnimationType(CANNON_BULLET_ANI_EXPLODE);
	}

	AnimatedGameObject::Render();
}

