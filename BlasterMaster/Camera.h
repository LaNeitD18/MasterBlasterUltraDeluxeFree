#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "useful_stuff.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include <D3DX10math.h>

// NAK son
// NAK tien

class Camera
{
public:
	Camera(Point size);
	~Camera();

	RESULT Frame();

	RESULT Initialize();
	RESULT Release();

	bool IsVisibleToCamera(BoundingBox& bbox) { return box.IsOverlap(bbox); }

	BoundingBox box;

	int screen_width;
	int screen_height;

	//LeSon

		//center of camera
	void SetPosition(float x, float y);
	void SetPosition(Point pos);
	int GetWidth();
	int GetHeight();
	Point GetPosition();
	RECT GetBound(); // change to BoundingBox later if you can

	//LeSon
	void SnapToBoundary();
	void FollowTarget();
	void SetTarget(Player* target);
	void SetCameraBoundary(RECT boundary);
	void SetCameraBoundary(int left, int top, int right, int bot);

private:
	int	mWidth,mHeight; // substitute for screen_width and screen_height (use private and get set please)

	Point mPosition;

	//LeSon
	//static Camera* __instance;
	Player* target;
	RECT boundary;
};

#endif // CAMERACLASS_H
