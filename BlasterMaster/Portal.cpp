//#include "Portal.h"
//
//
//CPortal::CPortal(float l, float t, float r, float b, int scene_id )
//{
//	this->scene_id = scene_id;
//	pos.x = l; 
//	pos.y = t;
//	width = r - l + 1;
//	height = b - t + 1;
//}
//
//void CPortal::Render()
//{
//	RenderBoundingBox();
//}
//
//void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
//{
//	l = pos.x;
//	t = pos.y;
//	r = pos.x + width;
//	b = pos.y + height;
//}