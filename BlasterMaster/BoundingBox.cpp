#include "BoundingBox.h"
#include "Utils.h"
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

bool BoundingBox::IsValid()
{
	return l != r || t != b;
}

double BoundingBox::SweptAABB(BoundingBox movingBox, Point v,
	bool& top, bool& left, bool& bottom, bool& right)
{
	if (!IsValid())
		return -INFINITY;
	if (!movingBox.IsValid())
		return -INFINITY;

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

	if (!(entryTime <= exitTime && (entryTime >= 0.0 || exitTime >= 0.0) && entryTime <= 1.0)) {
		return -INFINITY;
	}
	/*
	float overlapAreaX = min(this->r, movingBox.r + v.x) - max(this->l, movingBox.l + v.x);
	float overlapAreaY = min(this->b, movingBox.b + v.y) - max(this->t, movingBox.t + v.y);
	float overlapAreaXprev = min(this->r, movingBox.r) - max(this->l, movingBox.l);
	float overlapAreaYprev = min(this->b, movingBox.b) - max(this->t, movingBox.t);
	//*/
	/*
	float overlapAreaX = min(this->r, movingBox.r) - max(this->l, movingBox.l);
	float overlapAreaY = min(this->b, movingBox.b) - max(this->t, movingBox.t);
	//*/

	/*
	int result1 = -1;
	if (overlapAreaX <= overlapAreaY)
	{
		if (GetCenter().x > movingBox.GetCenter().x)
		{
			right = true;
			result1 = 1;
		}
		if (GetCenter().x < movingBox.GetCenter().x)
		{
			left = true;
			result1 = 3;
		}
	}
	else
	{
		if (GetCenter().y < movingBox.GetCenter().y)
		{
			top = true;
			result1 = 2;
		}
		if (GetCenter().y > movingBox.GetCenter().y)
		{
			bottom = true;
			result1 = 4;
		}
	}
	//*/
	//*

	if (xEntry == -INFINITY && yEntry == -INFINITY)
	{
		if (movingBox.b - t == 0)
		{
			bottom = true;
			//if (result1 != 4) DebugOut(L"Diff: %d , 4\n", result1);
		}
		if (movingBox.t - b == 0)
		{
			top = true;
			//if (result1 != 2) DebugOut(L"Diff: %d , 2\n", result1);
		}
		if (movingBox.r - l == 0)
		{
			right = true;
			//if (result1 != 1) DebugOut(L"Diff: %d , 1\n", result1);
		}
		if (movingBox.l - r == 0)
		{
			left = true;
			//if (result1 != 3) DebugOut(L"Diff: %d , 3\n", result1);
		}
	}
	else if (xEntry > yEntry)
	{
		if (movingBox.b - t == 0)
		{
			bottom = true;
			//if (result1 != 4) DebugOut(L"Diff: %d , 4\n", result1);
		}
		else if (movingBox.t - b == 0)
		{
			top = true;
			//if (result1 != 2) DebugOut(L"Diff: %d , 2\n", result1);
		}
		else if (xInvEntry > 0.0f)
		{
			left = true;
			//if (result1 != 3) DebugOut(L"Diff: %d , 3\n", result1);
		}
		else
		{
			right = true;
			//if (result1 != 1) DebugOut(L"Diff: %d , 1\n", result1);
		}
	}
	else
	{
		if (movingBox.r - l == 0)
		{
			right = true;
			//if (result1 != 1) DebugOut(L"Diff: %d , 1\n", result1);
		}
		else if (movingBox.l - r == 0)
		{
			left = true;
			//if (result1 != 3) DebugOut(L"Diff: %d , 3\n", result1);
		}
		else if (yInvEntry > 0.0f)
		{
			top = true;
			//if (result1 != 2) DebugOut(L"Diff: %d , 2\n", result1);
		}
		else 
		{
			bottom = true;
			//if (result1 != 4) DebugOut(L"Diff: %d , 4\n", result1);
		}
	}
	/*/
	DEBUG(
	if (!(((left || right) ^ (top || bottom)) && ((left ^ right) || (top ^ bottom))))
		DebugOut(L"%d %d %d %d\n", left, right, top, bottom);
	)//*/

	if (entryTime == -INFINITY)
		entryTime = -1000000;

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
