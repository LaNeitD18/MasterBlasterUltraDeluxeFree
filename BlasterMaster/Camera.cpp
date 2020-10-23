#include "Camera.h"

Camera::Camera(Point size) {
	/*screen_height = size.y;
	screen_width = size.x;*/

	mWidth = size.x;
	mHeight = size.y;
	mPosition = Point(0, 0);
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

int Camera::GetWidth()
{
	return mWidth;
}

int Camera::GetHeight()
{
	return mHeight;
}
