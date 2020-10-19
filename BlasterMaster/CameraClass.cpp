#include "CameraClass.h"

CameraClass::CameraClass(Point size) {
	screen_height = size.y;
	screen_width = size.x;
}

CameraClass::~CameraClass() {}

RESULT CameraClass::Frame()
{
    return 0;
}

RESULT CameraClass::Initialize()
{
    return 0;
}

RESULT CameraClass::Release()
{
    delete this;
    return 0;
}
