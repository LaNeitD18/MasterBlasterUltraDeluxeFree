#include "BoundingBox.h"

RECT BoundingBox::toRect()
{
	RECT rect;
	rect.top = this->t;
	rect.left = this->l;
	rect.right = this->r;
	rect.bottom = this->b;
	return rect;
}
