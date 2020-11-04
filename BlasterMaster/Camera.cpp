#include "Camera.h"

Camera::Camera(Point size) {
	/*screen_height = size.y;
	screen_width = size.x;*/

	mWidth = size.x;
	mHeight = size.y;
	mPosition = Point(0, 0);
	boundary.left = 0;
	boundary.right = 2048;
	boundary.top = 0;
	boundary.bottom = 2048;
}

Camera::~Camera() {}

RESULT Camera::Frame()
{
    return 0;
}

RESULT Camera::Initialize()
{
    return 0;
}

RESULT Camera::Release()
{
    delete this;
    return 0;
}

void Camera::SetPosition(float x, float y)
{
	SetPosition(Point(x, y));
}

void Camera::SetPosition(Point pos)
{
	mPosition = pos;
}

Point Camera::GetPosition()
{
	return mPosition;
}

RECT Camera::GetBound()
{
	RECT bound;
	bound.left = mPosition.x - mWidth / 2;
	bound.right = bound.left + mWidth;;
	bound.top = mPosition.y - mHeight / 2;
	bound.bottom = bound.top + mHeight;
	return bound;
}

void Camera::SnapToBoundary()
{
	RECT cameraRECT = GetBound();

	if (cameraRECT.left < boundary.left)
	{
		mPosition.x += boundary.left - cameraRECT.left;
	}

	if (cameraRECT.right > boundary.right)
	{
		mPosition.x -= cameraRECT.right - boundary.right;
	}

	if (cameraRECT.top < boundary.top)
	{
		mPosition.y += boundary.top - cameraRECT.top;
	}

	if (cameraRECT.bottom > boundary.bottom)
	{
		mPosition.y -= cameraRECT.bottom - boundary.bottom;
	}
}

void Camera::FollowTarget()
{
	if (target == NULL) return;
	SetPosition(target->GetPosition().x, target->GetPosition().y);
}

void Camera::SetTarget(Player * target)
{
	this->target = target;
}

void Camera::SetCameraBoundary(RECT boundary)
{
	this->boundary = boundary;
}

void Camera::SetCameraBoundary(int left, int top, int right, int bot)
{
	boundary.left = left;
	boundary.right - right;
	boundary.top = top;
	boundary.bottom = bot;
}

int Camera::GetWidth()
{
	return mWidth;
}

int Camera::GetHeight()
{
	return mHeight;
}


