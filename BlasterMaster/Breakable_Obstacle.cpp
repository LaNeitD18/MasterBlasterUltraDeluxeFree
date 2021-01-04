#include "Breakable_Obstacle.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"

static D3DCOLOR appearColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };

Breakable_Obstacle::Breakable_Obstacle()
{
	
}

Breakable_Obstacle::Breakable_Obstacle(float x, float y, int type)
{
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(BREAKABLETREE_ANIMATION_SET_ID));
	this->isOut = false;
	this->type = type;
	//SetState(ITEM_POWER_STATE_NORMAL);

	//time = 0;
}

BoundingBox Breakable_Obstacle::GetBoundingBox()
{
	if (!isOut) {
		float left = pos.x + OBS_BBOX_OFFSET_LEFT;
		float top = pos.y + OBS_BBOX_OFFSET_TOP;
		float right = pos.x + OBS_BBOX_OFFSET_RIGHT;
		float bottom = pos.y + OBS_BBOX_OFFSET_BOTTOM;
		return BoundingBox(left, top, right, bottom);
	}
	return BoundingBox(0, 0, 0, 0);
	
}

BoundingBox Breakable_Obstacle::GetBoundingBoxJason()
{
	if (!isOut) {
		float left = pos.x + OBS_BBOX_OFFSET_LEFT;
		float top = pos.y + OBS_BBOX_OFFSET_TOP;
		float right = pos.x + OBS_BBOX_OFFSET_RIGHT;
		float bottom = pos.y - OBS_BBOX_OFFSET_BOTTOM + 8;
		return BoundingBox(left, top, right, bottom);
	}
	return BoundingBox(0, 0, 0, 0);

}

#define POINT_START_FLASH 500
#define POINT_FINISH_FLASH 800

void Breakable_Obstacle::Update()
{
	//time++;
	//if (time >= POINT_START_FLASH) {
	//}
	//if (time > POINT_FINISH_FLASH) {
		//manager->RemoveElement(this);
	//}
}

void Breakable_Obstacle::Render()
{
	/*if (state == MINE_BULLET_STATE_NORMAL) {
		SetAnimationType(MINE_BULLET_ANI_NORMAL);
	}
	else if (state == MINE_BULLET_STATE_EXPLODE) {
		SetAnimationType(MINE_BULLET_ANI_EXPLODE);
	}*/
	if (!isOut) {
		drawArguments.SetColor(appearColor[1]);
	}
	else if (isOut) {
		drawArguments.SetColor(appearColor[0]);
	}
	SetAnimationType(0);
	AnimatedGameObject::Render();
}

bool Breakable_Obstacle::GetIsOut()
{
	return isOut;
}

void Breakable_Obstacle::SetIsOut(bool out)
{
	this->isOut = out;
}
