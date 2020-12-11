#include "Breakable_Tree.h"
#include "GameGlobal.h"
#include "Utils.h"
#include "Game.h"
#include "Camera.h"

static D3DCOLOR appearColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };

Breakable_Tree::Breakable_Tree()
{
	
}

Breakable_Tree::Breakable_Tree(float x, float y)
{
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(BREAKABLETREE_ANIMATION_SET_ID));
	this->isOut = false;
	//SetState(ITEM_POWER_STATE_NORMAL);

	//time = 0;
}

BoundingBox Breakable_Tree::GetBoundingBox()
{
	if (!isOut) {
		float left = pos.x + BREAKABLETREE_BBOX_OFFSET_LEFT;
		float top = pos.y + BREAKABLETREE_BBOX_OFFSET_TOP;
		float right = pos.x + BREAKABLETREE_BBOX_OFFSET_RIGHT;
		float bottom = pos.y + BREAKABLETREE_BBOX_OFFSET_BOTTOM;
		return BoundingBox(left, top, right, bottom);
	}
	return BoundingBox(0, 0, 0, 0);
	
}

BoundingBox Breakable_Tree::GetBoundingBoxJason()
{
	if (!isOut) {
		float left = pos.x + BREAKABLETREE_BBOX_OFFSET_LEFT;
		float top = pos.y + BREAKABLETREE_BBOX_OFFSET_TOP;
		float right = pos.x + BREAKABLETREE_BBOX_OFFSET_RIGHT;
		float bottom = pos.y - BREAKABLETREE_BBOX_OFFSET_BOTTOM + 8;
		return BoundingBox(left, top, right, bottom);
	}
	return BoundingBox(0, 0, 0, 0);

}

#define POINT_START_FLASH 500
#define POINT_FINISH_FLASH 800

void Breakable_Tree::Update()
{
	//time++;
	//if (time >= POINT_START_FLASH) {
	//}
	//if (time > POINT_FINISH_FLASH) {
		//manager->RemoveElement(this);
	//}
}

void Breakable_Tree::Render()
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

bool Breakable_Tree::GetIsOut()
{
	return isOut;
}

void Breakable_Tree::SetIsOut(bool out)
{
	this->isOut = out;
}
