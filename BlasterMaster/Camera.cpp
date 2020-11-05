#include "Camera.h"
#include "GameObject.h"

Camera* Camera::__instance = NULL;

Camera::Camera(Point size) {
	/*screen_height = size.y;
	screen_width = size.x;*/
	target = NULL;

	mWidth = size.x;
	mHeight = size.y;
	mPosition = Point(0, 0);
	boundary.l = 0;
	boundary.r = 2048;
	boundary.t = 0;
	boundary.b = 2048;
	if (__instance == NULL)
		__instance = this;
	else
		;//throw 1;
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

BoundingBox Camera::GetBound()
{
	BoundingBox bound;
	bound.l = mPosition.x - mWidth / 2.0;
	bound.r = bound.l + mWidth;
	bound.t = mPosition.y - mHeight / 2.0;
	bound.b = bound.t + mHeight;
	return bound;
}

void Camera::SnapToBoundary()
{
	BoundingBox cameraRECT = GetBound();

	if (cameraRECT.l < boundary.l)
	{
		mPosition.x += boundary.l - cameraRECT.l;
	}

	if (cameraRECT.r > boundary.r)
	{
		mPosition.x -= cameraRECT.r - boundary.r;
	}

	if (cameraRECT.t < boundary.t)
	{
		mPosition.y += boundary.t - cameraRECT.t;
	}

	if (cameraRECT.b > boundary.b)
	{
		mPosition.y -= cameraRECT.b - boundary.b;
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

void Camera::SetCameraBoundary(BoundingBox boundary)
{
	this->boundary = boundary;
}

void Camera::SetCameraBoundary(int left, int top, int right, int bot)
{
	boundary.l = left;
	boundary.r - right;
	boundary.t = top;
	boundary.b = bot;
}

void Camera::setCameraInstance(Camera * camera)
{
	//if (__instance == NULL)
		__instance = camera;
}

int Camera::GetWidth()
{
	return mWidth;
}

int Camera::GetHeight()
{
	return mHeight;
}


