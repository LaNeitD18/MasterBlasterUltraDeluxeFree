#include "BoundingBox.h"

bool BoundingBox::IsOverlap(BoundingBox other)
{
	float left = other.l - this->r;
	float top = other.b - this->t;
	float right = other.r - this->l;
	float bottom = other.t - this->b;

	// check collision
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

Point BoundingBox::GetCenter()
{
	return Point((l + r) / 2, (b + t) / 2);
}

RECT BoundingBox::toRect()
{
	RECT rect;
	rect.top = this->t;
	rect.left = this->l;
	rect.right = this->r;
	rect.bottom = this->b;
	return rect;
}
