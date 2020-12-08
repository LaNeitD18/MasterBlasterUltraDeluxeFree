#include "MiniRedBullet.h"
#include "Utils.h"
#include "Camera.h"
#include "GameGlobal.h"
#include "SceneArea2SideView.h"
#include "Game.h"

MiniRedBullet::MiniRedBullet()
{

}

MiniRedBullet::MiniRedBullet(Point pos, Point playerPos)
{
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
	Point distance = (playerPos - pos);

	v.x = distance.x / module *2;
	v.y = distance.y / module *3;

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(19));
	SetState(MINI_BULLET_STATE_NORMAL);
}

BoundingBox MiniRedBullet::GetBoundingBox()
{
	float left = pos.x + BULLET_BBOX_OFFSET_LEFT;
	float top = pos.y + BULLET_BBOX_OFFSET_TOP;
	float right = pos.x + BULLET_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + BULLET_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void MiniRedBullet::Update()
{
	pos += dx();

	if (wallBot || wallLeft || wallRight) {
		v = Point(0, 0);
		SetState(MINI_BULLET_STATE_EXPLODE);
	}

	if (!Camera::GetInstance()->GetBound().IsInsideBox(pos))
		//SetState(MINI_BULLET_STATE_EXPLODE);
		manager->RemoveElement(this);

	if (currentTime == 0 && state == MINI_BULLET_STATE_EXPLODE)
		manager->RemoveElement(this);
}

void MiniRedBullet::Render()
{
	if (state == MINI_BULLET_STATE_NORMAL) {
		SetAnimationType(MINI_BULLET_ANI_NORMAL);
	}
	else if (state == MINI_BULLET_STATE_EXPLODE)
		SetAnimationType(MINI_BULLET_ANI_EXPLODE);

	AnimatedGameObject::Render();
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS MiniRedBullet
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)