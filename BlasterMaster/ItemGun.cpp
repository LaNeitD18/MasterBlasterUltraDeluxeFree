#include "ItemGun.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"

static D3DCOLOR flashColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };
static D3DCOLOR specialRender[6] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,255), D3DCOLOR_ARGB(0,255,255,255) };

ItemGun::ItemGun()
{

}

ItemGun::ItemGun(Point pos)
{
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(ITEM_GUN_ANIMATION_SET_ID));
	//SetState(ITEM_GUN_STATE_NORMAL);
	type = 0;
	time = 0;
}

ItemGun::ItemGun(Point pos, int n)
{
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	/*int ani_id = 0;
	switch (n) {
	case 1:
		ani_id = ITEM_HOMING_ANIMATION_SET_ID;
		break;
	case 2:
		ani_id = ITEM_THUNDER_ANIMATION_SET_ID;
		break;
	case 3:
		ani_id = ITEM_MULTI_ANIMATION_SET_ID;
		break;
	default:
		break;
	}
	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(ani_id));*/
	//SetState(ITEM_GUN_STATE_NORMAL);
	type = n;
	time = 0;
}

BoundingBox ItemGun::GetBoundingBox()
{
	float left = pos.x + ITEM_GUN_BBOX_OFFSET_LEFT;
	float top = pos.y + ITEM_GUN_BBOX_OFFSET_TOP;
	float right = pos.x + ITEM_GUN_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + ITEM_GUN_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

#define POINT_START_FLASH 500
#define POINT_FINISH_FLASH 800

void ItemGun::Update()
{
}

void ItemGun::Render()
{
	/*if (state == MINE_BULLET_STATE_NORMAL) {
		SetAnimationType(MINE_BULLET_ANI_NORMAL);
	}
	else if (state == MINE_BULLET_STATE_EXPLODE) {
		SetAnimationType(MINE_BULLET_ANI_EXPLODE);
	}*/
	if (type != 0) {
		drawArguments.SetColor(specialRender[rand() % 6]);
	}
	SetAnimationType(0);
	AnimatedGameObject::Render();
}