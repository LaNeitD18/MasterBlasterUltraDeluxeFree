#pragma once
#include "useful_stuff.h"

// NAK son
// NAK tien

class BoundingBox
{
public:
	float l, r, t, b;
	bool IsInsideBox(Point pos);
	bool IsInsideBox(BoundingBox other);
	bool IsOverlap(BoundingBox other);
	Point GetCenter();
	BoundingBox(float l = 0, float t = 0, float r = 0, float b = 0)
		:l(l), t(t), r(r), b(b) {};
	// cast to rect
	RECT toRect();
};

