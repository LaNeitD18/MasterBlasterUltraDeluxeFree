#include "Dome.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"

Dome::Dome() {
	SetState(DOME_STATE_WALKING_LEFT);
}

Dome::Dome(float x, float y) {
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	drawArguments.SetRotationCenter(Point(0, 0));
	direction = Point(-1, 1);
	SetState(DOME_STATE_WALKING_LEFT);
}

BoundingBox Dome::GetBoundingBox()
{
	float left = pos.x + DOME_BBOX_OFFSET_LEFT;
	float top = pos.y + DOME_BBOX_OFFSET_TOP;
	float right = pos.x + DOME_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + DOME_BBOX_OFFSET_BOTTOM;
		
	return BoundingBox(left, top, right, bottom);
}

void Dome::Update()
{
	pos += dx();
	Enemy::Update();

	// lay vi tri player
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = Point();
	if (scene != nullptr) {
		playerPos = scene->GetTarget()->GetPosition();
	}

	// xet neu player va dome gan nhau theo x
	if (abs(pos.x - playerPos.x) <= 2) {
		// dome dang di tren wallBot va player phia tren dome
		if (wallBot && playerPos.y < pos.y - 20) {
			direction.y = -1;
			wallBot = false;
			SetState(DOME_STATE_JUMPING_VERTICAL);
		}
		// dome tren wall top va player phia duoi dome
		if (wallTop && playerPos.y > pos.y) {
			direction.y = 1;
			wallTop = false;
			SetState(DOME_STATE_JUMPING_VERTICAL);
		}
	}
	// TH phong theo chieu ngang
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

	// set rotation tùy theo vị trí hiện tại
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
	// đang đi ngang, xét TH theo x là state hiện tại, y là state trước đó
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
	// đang đi dọc, xét TH theo y là state hiện tại, x là state trước đó
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
	// nếu gặp wallLeft, đi như bth
	if (wallLeft) {
		SetStateByDirection();
	}
	// nếu đi ngang hết wall mà ko gặp wallLeft, thì đổi ngược dir x, y 
	// để luôn đóng khung dome và quy định nó đi bên trong khung --> set state mới phù hợp
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
		DebugOut(L"left\n");
		break;
	case DOME_STATE_WALKING_RIGHT:
		direction.x = 1;
		v.x = speedX * direction.x;
		v.y = 0;
		DebugOut(L"right\n");
		break;
	case DOME_STATE_WALKING_UP:
		direction.y = -1;
		v.x = 0;
		v.y = speedY * direction.y;
		DebugOut(L"up\n");
		break;
	case DOME_STATE_WALKING_DOWN:
		direction.y = 1;
		v.x = 0;
		v.y = speedY * direction.y;
		DebugOut(L"down\n");
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