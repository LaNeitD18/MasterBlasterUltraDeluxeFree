#include "Dome.h"

Dome::Dome() {
	SetState(DOME_STATE_WALKING);
}

Dome::Dome(float x, float y) {
	SetState(DOME_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH, DOME_BBOX_HEIGHT));
	direction = Point(-1.0, -1.0);
}

BoundingBox Dome::GetBoundingBox()
{
	float left = pos.x - DOME_BBOX_WIDTH;
	float top = pos.y - DOME_BBOX_HEIGHT;
	float right = pos.x + DOME_BBOX_WIDTH;
	float bottom;

	if (state == DOME_STATE_DIE)
		bottom = pos.y + DOME_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + DOME_BBOX_HEIGHT;
	return BoundingBox(left, top, right, bottom);
}

void Dome::Update()
{
	pos += dx();

	/*if (v.x < 0 && wallLeft) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN);
		previousVelocity.x = v.x;
		v.x = 0;
		v.y = -previousVelocity.y;
	}
	if (v.x > 0 && wallRight) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 3);
		previousVelocity.x = v.x;
		v.x = 0;
		v.y = -previousVelocity.y;
	}
	if (v.y < 0 && wallTop) {
		drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN*2);
		previousVelocity.y = v.y;
		v.y = 0;
		v.x = -previousVelocity.x;
	}
	if (v.y > 0) {
		if (wallBot) {
			drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 0);
			v.y = 0;
			v.x = -previousVelocity.x;
		}
		else if (!wallBot && !wallRight) {
			drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 0);
		}
	}*/
	if (v.y < 0 && wallTop) {
		WalkRight();
	}
	if (v.y > 0 && wallBot) {
		WalkLeft();
	}
	if (v.x < 0 && wallLeft) {
		WalkUp();
	}
	if (v.x > 0 && wallRight) {
		WalkDown();
	}
	if (v.y > 0 && !wallBot && !wallRight && direction.x == 1) {
		drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH * 2, DOME_BBOX_HEIGHT * 2));
		WalkRight();
	}
	if (v.y < 0 && !wallTop && !wallLeft && direction.x == 1) {
		drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH * 2, DOME_BBOX_HEIGHT * 2));
		WalkLeft();
	}
	if (v.x > 0 && !wallTop && !wallRight && direction == Point(1,1)) {
		drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH * 2, DOME_BBOX_HEIGHT * 2));
		WalkUp();
	}
	if (v.x < 0 && !wallLeft && !wallBot && direction.y == 1) {
		drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH * 2, DOME_BBOX_HEIGHT * 2));
		WalkDown();
	}
	drawArguments.SetRotationCenter(Point(DOME_BBOX_WIDTH, DOME_BBOX_HEIGHT));
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Dome::Render()
{
	SetAnimationType(DOME_ANI_WALKING);
	/*if (state == TELEPORTER_STATE_DIE) {
		ani = TELEPORTER_ANI_DIE;
	}*/

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Dome::WalkRight()
{
	drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 2);
	v.y = 0;
	v.x = -previousVelocity.x;
	direction.x = 1;
}

void Dome::WalkLeft()
{
	drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 0);
	v.y = 0;
	v.x = previousVelocity.x;
	direction.x = -1;
}

void Dome::WalkUp()
{
	drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN);
	v.x = 0;
	v.y = previousVelocity.y;
	direction.y = -1;
}

void Dome::WalkDown()
{
	drawArguments.SetRotation(ROTATE_90DEGREE_TO_RADIAN * 3);
	v.x = 0;
	v.y = -previousVelocity.y;
	direction.y = 1;
}

void Dome::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case DOME_STATE_DIE:
		pos.y += DOME_BBOX_HEIGHT - DOME_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case DOME_STATE_WALKING:
		v.x = -DOME_WALKING_SPEED_X;

		previousVelocity.x = -DOME_WALKING_SPEED_X;
		previousVelocity.y = -DOME_WALKING_SPEED_Y;
	}

}


#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Dome
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)