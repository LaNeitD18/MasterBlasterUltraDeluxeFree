#include "BoundingBox.h"
#include <algorithm>

bool BoundingBox::IsInsideBox(Point pos)
{
	return pos.x > l && pos.x < r && pos.y > t && pos.y < b;
}

bool BoundingBox::IsInsideBox(BoundingBox other)
{
	return 
		this->r <= other.r &&
		this->b <= other.b &&
		this->l >= other.l &&
		this->t >= other.t;
}

bool BoundingBox::IsOverlap(BoundingBox other)
{
	float left = other.l - this->r;
	float top = other.b - this->t;
	float right = other.r - this->l;
	float bottom = other.t - this->b;

	// check collision
	return !(left > 0 || right < 0 || top < 0 || bottom > 0);
}

double BoundingBox::SweptAABB(BoundingBox movingBox, Point v,
	bool& top, bool& left, bool& bottom, bool& right)
{
	double xInvEntry, yInvEntry;
	double xInvExit, yInvExit;

	// find the distance between the objects on the near and far sides for both x and y 
	if (v.x <= 0.0f)
	{
		xInvEntry = movingBox.l - r;
		xInvExit = movingBox.r - l;
	}
	else
	{
		xInvEntry = movingBox.r - l;
		xInvExit = movingBox.l - r;
	}

	if (v.y <= 0.0f)
	{
		yInvEntry = movingBox.t - b;
		yInvExit = movingBox.b - t;
	}
	else
	{
		yInvEntry = movingBox.b - t;
		yInvExit = movingBox.t - b;
	}

	double xEntry, yEntry;
	double xExit, yExit;

	if (v.x == 0.0f)
	{
		xEntry = -INFINITY;
		xExit = INFINITY;
		// If x not overlap
		if (xInvEntry > 0 || xInvExit < 0)
			std::swap(xEntry, xExit);
	}
	else
	{
		xEntry = -xInvEntry / v.x;
		xExit = -xInvExit / v.x;
	}

	if (v.y == 0.0f)
	{
		yEntry = -INFINITY;
		yExit = INFINITY;
		// If y not overlap
		if (yInvEntry > 0 || yInvExit < 0)
			std::swap(yEntry, yExit);
	}
	else
	{
		yEntry = -yInvEntry / v.y;
		yExit = -yInvExit / v.y;
	}

	double entryTime = max(xEntry, yEntry);
	double exitTime = min(xExit, yExit);

	if (entryTime > exitTime || (entryTime < 0.0 && exitTime < 0.0) || entryTime > 1.0) {
		return -INFINITY;
	}
	//*
	float overlapAreaX = min(this->r, movingBox.r + v.x) - max(this->l, movingBox.l + v.x);
	float overlapAreaY = min(this->b, movingBox.b + v.y) - max(this->t, movingBox.t + v.y);
	/*/
	float overlapAreaX = min(this->r, movingBox.r) - max(this->l, movingBox.l);
	float overlapAreaY = min(this->b, movingBox.b) - max(this->t, movingBox.t);
	//*/

	if (overlapAreaX <= overlapAreaY)
	{
		if (GetCenter().x > movingBox.GetCenter().x)
			right = true;
		else
			left = true;
	}
	else
	{
		if (GetCenter().y < movingBox.GetCenter().y)
			top = true;
		else
			bottom = true;
	}

	return entryTime;
}

double BoundingBox::SweptAABB(BoundingBox movingBox, Point v)
{
	bool discard;
	return SweptAABB(movingBox, v, discard, discard, discard, discard);
}

void BoundingBox::Move(Point direction)
{
	l += direction.x;
	r += direction.x;
	t += direction.y;
	b += direction.y;
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
