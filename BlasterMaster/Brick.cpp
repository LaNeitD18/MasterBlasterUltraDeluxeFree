#include "Brick.h"

void CBrick::Render()
{
	//animationSet->at(0)->Render(pos);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(BoundingBox& box)
{
	float l = pos.x;
	float t = pos.y;
	float r = pos.x + BRICK_BBOX_WIDTH;
	float b = pos.y + BRICK_BBOX_HEIGHT;
	box = BoundingBox(l, t, r, b);
}