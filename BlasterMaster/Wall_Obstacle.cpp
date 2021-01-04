//#include "Wall_Obstacle.h"
//#include "GameGlobal.h"
//#include "Utils.h"
//#include "Game.h"
//#include "Camera.h"
//
//static D3DCOLOR appearColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };
//
//Wall_Obstacle::Wall_Obstacle()
//{
//
//}
//
//Wall_Obstacle::Wall_Obstacle(float x, float y)
//{
//	pos = Point(x, y);
//	drawArguments.SetScale(D3DXVECTOR2(1, 1));
//
//	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(WALLO_ANIMATION_SET_ID));
//	this->isOut = false;
//	//SetState(ITEM_POWER_STATE_NORMAL);
//
//	//time = 0;
//}
//
//BoundingBox Wall_Obstacle::GetBoundingBox()
//{
//	if (!isOut) {
//		float left = pos.x + WALLO_BBOX_OFFSET_LEFT;
//		float top = pos.y + WALLO_BBOX_OFFSET_TOP;
//		float right = pos.x + WALLO_BBOX_OFFSET_RIGHT;
//		float bottom = pos.y + WALLO_BBOX_OFFSET_BOTTOM;
//		return BoundingBox(left, top, right, bottom);
//	}
//	return BoundingBox(0, 0, 0, 0);
//
//}
//
//void Wall_Obstacle::Update()
//{
//
//}
//
//void Wall_Obstacle::Render()
//{
//	if (!isOut) {
//		drawArguments.SetColor(appearColor[1]);
//	}
//	else if (isOut) {
//		drawArguments.SetColor(appearColor[0]);
//	}
//	SetAnimationType(0);
//	AnimatedGameObject::Render();
//}
//
//bool Wall_Obstacle::GetIsOut()
//{
//	return isOut;
//}
//
//void Wall_Obstacle::SetIsOut(bool out)
//{
//	this->isOut = out;
//}
