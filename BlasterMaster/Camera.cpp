#include "Camera.h"

Camera::Camera(Point size) {
	screen_height = size.y;
	screen_width = size.x;
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
