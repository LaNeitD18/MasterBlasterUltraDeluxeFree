#include "Brick.h"

void CBrick::Render()
{
	//animationSet->at(0)->Render(pos);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = pos.x;
	t = pos.y;
	r = pos.x + BRICK_BBOX_WIDTH;
	b = pos.y + BRICK_BBOX_HEIGHT;
}