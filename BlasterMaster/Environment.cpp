#include "Environment.h"

BoundingBox Environment::GetBoundingBox(BoundingBox& box)
{
	return BoundingBox(box.l, box.t, box.r, box.b);
}

