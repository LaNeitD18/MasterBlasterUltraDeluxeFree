#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "useful_stuff.h"
#include "BoundingBox.h"
#include <D3DX10math.h>

// NAK son
// NAK tien

class CameraClass
{
    public:
        CameraClass(Point size);
        ~CameraClass();

        RESULT Frame();

        RESULT Initialize();
        RESULT Release();

		bool IsVisibleToCamera(BoundingBox& bbox) { return box.IsOverlap(bbox); }

		BoundingBox box;

		int screen_width;
		int screen_height;
		Point pos;
};

#endif // CAMERACLASS_H
