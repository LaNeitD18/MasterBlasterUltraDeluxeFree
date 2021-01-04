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
	bool IsValid();

	// Expand the BBox in the direction
	BoundingBox& Drag(Point dir);

	// Returns the time it takes till collision
	// Returns -INFINITY if it is not colliding
	// Returns -10000 if it is overlapping
	double SweptAABB(BoundingBox movingBox, Point v, 
		bool& top, bool& left, bool& bottom, bool& right);
	double SweptAABB(BoundingBox movingBox, Point v);

	void Move(Point direction);
	Point GetCenter();
	BoundingBox(float l = 0, float t = 0, float r = 0, float b = 0)
		:l(l), t(t), r(r), b(b) {};
	// cast to rect
	RECT toRect();
};

