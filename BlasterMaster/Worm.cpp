#include "Worm.h"
#include "Utils.h"

Worm::Worm() {
	SetState(WORM_STATE_WALKING);
}

Worm::Worm(float x, float y)
{
	SetState(WORM_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(0.25, 0.25));
}

void Worm::GetBoundingBox(BoundingBox& box)
{
	float left = pos.x;
	float top = pos.y;
	float right = pos.x + WORM_BBOX_WIDTH;
	float bottom;
	if (state == WORM_STATE_DIE)
		bottom = pos.y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WORM_BBOX_HEIGHT;
	box = BoundingBox(left, top, right, bottom);
}

void Worm::Update()
{
	pos += dx();

	if (v.x < 0 && pos.x < 0) {
		pos.x = 0; v.x = -v.x;
		DebugOut(L"sZZZ\n");


		//displayMessage(isFlipHorizontal);
	}

	if (v.x > 0 && pos.x > 80) {
		pos.x = 80; v.x = -v.x;
		DebugOut(L"sZZZ\n");

		
		//displayMessage(isFlipHorizontal);
	}
	/*else {
		isFlipHorizontal = false;
		displayMessage(isFlipHorizontal);

	}*/

}

void Worm::Render()
{
	if (state == WORM_STATE_DIE) {
		SetAnimationType(WORM_ANI_DIE);
	}
	else if (v.x > 0) {
		SetAnimationType(WORM_ANI_WALKING_RIGHT);
		isFlipHorizontal = false;
	}
	else if (v.x <= 0) isFlipHorizontal = true;

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Worm::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case WORM_STATE_DIE:
		pos.y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case WORM_STATE_WALKING:
		v.x = WORM_WALKING_SPEED;
	}

}