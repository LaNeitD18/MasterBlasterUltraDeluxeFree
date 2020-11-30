#include "Dome.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "time.h"

Dome::Dome() {
	SetState(DOME_STATE_WALKING_LEFT);
}

Dome::Dome(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	drawArguments.SetRotationCenter(Point(0, 0));
	direction = Point(-1, 1);
	SetState(DOME_STATE_WALKING_LEFT);
	drawArguments.SetRotation(4);
}

BoundingBox Dome::GetBoundingBox()
{
	float left = pos.x + DOME_BBOX_OFFSET_LEFT;
	float top = pos.y + DOME_BBOX_OFFSET_TOP;
	float right = pos.x + DOME_BBOX_OFFSET_RIGHT;
	float bottom;

	if (state == DOME_STATE_DIE)
		bottom = pos.y + DOME_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + DOME_BBOX_OFFSET_BOTTOM;
	return BoundingBox(left, top, right, bottom);
}

void Dome::Update()
{
	pos += dx();
	Enemy::Update();

	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = scene->GetTarget()->GetPosition();

	if (abs(pos.x - playerPos.x) <= 2 && abs(pos.y - playerPos.y) >= 20) {
		if (wallBot) {
			direction.y = -1;
			wallBot = false;
			SetState(DOME_STATE_JUMPING_VERTICAL);
		}
		if (wallTop) {
			direction.y = 1;
			wallTop = false;
			SetState(DOME_STATE_JUMPING_VERTICAL);
		}
	}
	else if (abs(pos.y - playerPos.y) <= 2) {
		if (wallLeft) {
			direction.x = 1;
			wallLeft = false;
			SetState(DOME_STATE_JUMPING_HORIZONTAL);
		}
		if (wallRight) {
			direction.x = -1;
			wallRight = false;
			SetState(DOME_STATE_JUMPING_HORIZONTAL);
		}
		
	}

	if (state == DOME_STATE_WALKING_LEFT) {
		WalkLeft();
	}
	else if (state == DOME_STATE_WALKING_RIGHT) {
		WalkRight();
	}
	else if (state == DOME_STATE_WALKING_UP) {
		WalkUp();
	}
	else if (state == DOME_STATE_WALKING_DOWN) {
		WalkDown();
	}
	else if (state == DOME_STATE_JUMPING_VERTICAL) {
		JumpVertically();
	}
	else if (state == DOME_STATE_JUMPING_HORIZONTAL) {
		JumpHorizontally();
	}

	if (wallBot) {
		rotation = ROTATE_90DEGREE_TO_RADIAN * 0;
	}
	else if (wallLeft) {
		rotation = ROTATE_90DEGREE_TO_RADIAN;
	}
	else if (wallTop) {
		rotation = ROTATE_90DEGREE_TO_RADIAN * 2;
	}
	else if (wallRight) {
		rotation = ROTATE_90DEGREE_TO_RADIAN * 3;
	}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Dome::Render()
{
	SetAnimationType(DOME_ANI_WALKING);

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Dome::SetStateByDirection() {
	if (v.x != 0) {
		if (direction == Point(-1, 1) || direction == Point(1, 1)) {
			pos.y -= 2;
			SetState(DOME_STATE_WALKING_UP);
		}
		else if (direction == Point(-1, -1) || direction == Point(1, -1)) {
			pos.y += 2;
			SetState(DOME_STATE_WALKING_DOWN);
		}
	}

	else if (v.y != 0) {
		if (direction == Point(-1, 1) || direction == Point(-1, -1)) {
			pos.x += 2;
			SetState(DOME_STATE_WALKING_RIGHT);
		}
		else if (direction == Point(1, 1) || direction == Point(1, -1)) {
			pos.x -= 2;
			SetState(DOME_STATE_WALKING_LEFT);
		}
	}
}

void Dome::WalkLeft()
{
	if (wallLeft) {
		SetStateByDirection();
	}
	else {
		if (!wallBot && !wallTop) {
			direction.x = -direction.x;
			direction.y = -direction.y;
			SetStateByDirection();
		}
	}
}

void Dome::WalkUp()
{
	if (wallTop) {
		SetStateByDirection();
	}
	else {
		if (!wallLeft && !wallRight) {
			direction.x = -direction.x;
			direction.y = -direction.y;
			SetStateByDirection();
		}
	}
}

void Dome::WalkRight()
{
	if (wallRight) {
		SetStateByDirection();
	}
	else {
		if (!wallBot && !wallTop) {
			direction.x = -direction.x;
			direction.y = -direction.y;
			SetStateByDirection();
		}
	}
}

void Dome::WalkDown()
{
	if (wallBot) {
		SetStateByDirection();
	}
	else {
		if (!wallLeft && !wallRight) {
			direction.x = -direction.x;
			direction.y = -direction.y;
			SetStateByDirection();
		}
	}
}

void Dome::JumpVertically() 
{
	//pos.y += (5 * direction.y);
	if (wallBot || wallTop) {
		SetStateByDirection();
	}

}

void Dome::JumpHorizontally()
{
	//pos.x += (5 * direction.x);
	if (wallLeft || wallRight) {
		SetStateByDirection();
	}
}

void Dome::SetState(int state)
{
	GameObject::SetState(state);
	float speedX = DOME_WALKING_SPEED_X;
	float speedY = DOME_WALKING_SPEED_Y;

	switch (state)
	{
	case DOME_STATE_DIE:
		pos.y += DOME_BBOX_HEIGHT - DOME_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case DOME_STATE_WALKING_LEFT:
		direction.x = -1;
		v.x = speedX * direction.x;
		v.y = 0;
		break;
	case DOME_STATE_WALKING_RIGHT:
		direction.x = 1;
		v.x = speedX * direction.x;
		v.y = 0;
		break;
	case DOME_STATE_WALKING_UP:
		direction.y = -1;
		v.x = 0;
		v.y = speedY * direction.y;
		break;
	case DOME_STATE_WALKING_DOWN:
		direction.y = 1;
		v.x = 0;
		v.y = speedY * direction.y;
		break;
	case DOME_STATE_JUMPING_VERTICAL:
		v.x = 0;
		v.y = speedY * 5 * direction.y;
		break;
	case DOME_STATE_JUMPING_HORIZONTAL:
		v.x = speedX * 5 * direction.x;
		v.y = 0;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Dome
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)