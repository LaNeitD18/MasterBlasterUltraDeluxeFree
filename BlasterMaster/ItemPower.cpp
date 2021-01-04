#include "ItemPower.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"

static D3DCOLOR flashColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };
static D3DCOLOR specialRender[6] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,255), D3DCOLOR_ARGB(0,255,255,255) };

ItemPower::ItemPower()
{

}

ItemPower::ItemPower(Point pos, int special)
{
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(ITEM_POWER_ANIMATION_SET_ID));
	//SetState(ITEM_POWER_STATE_NORMAL);

	this->special = special;
	time = 0;
}

BoundingBox ItemPower::GetBoundingBox()
{
	float left = pos.x + ITEM_POWER_BBOX_OFFSET_LEFT;
	float top = pos.y + ITEM_POWER_BBOX_OFFSET_TOP;
	float right = pos.x + ITEM_POWER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + ITEM_POWER_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

#define POINT_START_FLASH 500
#define POINT_FINISH_FLASH 800

void ItemPower::Update()
{
	time++;
	if (time >= POINT_START_FLASH) {
		drawArguments.SetColor(flashColor[rand() % 2]);
	}
	if (time > POINT_FINISH_FLASH) {
		manager->RemoveElement(this);
	}
}

void ItemPower::Render()
{
	/*if (state == MINE_BULLET_STATE_NORMAL) {
		SetAnimationType(MINE_BULLET_ANI_NORMAL);
	}
	else if (state == MINE_BULLET_STATE_EXPLODE) {
		SetAnimationType(MINE_BULLET_ANI_EXPLODE);
	}*/
	if (special == 1) {
		drawArguments.SetColor(specialRender[rand() % 6]);
	}
	SetAnimationType(0);
	AnimatedGameObject::Render();
}