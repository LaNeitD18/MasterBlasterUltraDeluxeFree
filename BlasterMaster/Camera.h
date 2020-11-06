#ifndef CAMERACLASS_H
#define CAMERACLASS_H

#include "useful_stuff.h"
#include "BoundingBox.h"
#include <D3DX10math.h>

// NAK son
// NAK tien
class Player;

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
	BoundingBox GetBound(); // change to BoundingBox later if you can

	//LeSon
	void SnapToBoundary();
	void FollowTarget();
	void SetTarget(Player* target);
	void SetCameraLimitarea(BoundingBox boundary);
	void SetCameraLimitarea(float left, float top, float right, float bot);

	//singleton
	static Camera* GetInstance()
	{
		if (__instance == NULL)
		{
			__instance = new Camera(Point(0,0));
		}
		return __instance;
	}
	static void setCameraInstance(Camera* camera);

private:
	int	mWidth,mHeight; // substitute for screen_width and screen_height (use private and get set please)

	Point mPosition;

	//LeSon
	static Camera* __instance;
	Player* target;
	BoundingBox boundary;
};

#endif // CAMERACLASS_H
