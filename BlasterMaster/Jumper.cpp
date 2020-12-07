#include "Jumper.h"
#include "time.h"

Jumper::Jumper() {
	SetState(JUMPER_STATE_WALKING);
}

Jumper::Jumper(float x, float y) {
	SetState(JUMPER_STATE_WALKING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	jumpRange = 0;
	countDownTimeToJump = COUNTDOWN_TIME_TO_JUMP;
	v.x = JUMPER_WALKING_SPEED;
}

BoundingBox Jumper::GetBoundingBox()
{
	float left = pos.x + JUMPER_BBOX_OFFSET_LEFT;
	float top = pos.y + JUMPER_BBOX_OFFSET_TOP;
	float right = pos.x + JUMPER_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + JUMPER_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Jumper::Jump()
{
	if (jumpRange > 40) {
		SetState(JUMPER_STATE_FALLING);
	}
}

void Jumper::Walk()
{
	srand(time(NULL));

	countDownTimeToJump--;
	if (countDownTimeToJump == 0) {
		countJump = rand() % 4 + 2;
		SetState(JUMPER_STATE_JUMPING);
	}

	if (!wallBot) {
		SetState(JUMPER_STATE_FALLING);
	}
}

void Jumper::Fall()
{
	if (wallBot) {
		jumpRange = 0;
		/*countDownTimeToJump = COUNTDOWN_TIME_TO_JUMP;
		SetState(JUMPER_STATE_WALKING);*/

		countJump--;
		if (countJump == 0) {
			SetState(JUMPER_STATE_WALKING);
			countDownTimeToJump = COUNTDOWN_TIME_TO_JUMP;
		}
		else {
			SetState(JUMPER_STATE_JUMPING);
		}
	}
}

void Jumper::Update()
{
	pos += dx();
	Enemy::Update();

	if (state == JUMPER_STATE_JUMPING) {
		jumpRange += abs(dx().y);
		Jump();
	}
	else if (state == JUMPER_STATE_WALKING) {
		Walk();
	}
	else if (state == JUMPER_STATE_FALLING) {
		Fall();
	}

	if (wallLeft || wallRight) {
		pos.x -= dx().x * 2;
		v.x = -v.x;
	}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Jumper::Render()
{
	if (state == JUMPER_STATE_DIE) {
		SetAnimationType(JUMPER_ANI_DIE);
	}
	else SetAnimationType(JUMPER_ANI_WALK);

	if (v.x > 0) {
		isFlipVertical = false;
	}
	else if (v.x < 0) {
		isFlipVertical = true;
	}

	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Jumper::SetState(int state)
{
	GameObject::SetState(state);
	switch (state)
	{
	case JUMPER_STATE_DIE:
		v.x = 0;
		v.y = 0;
		break;
	case JUMPER_STATE_WALKING:
		v.y = 0;
		break;
	case JUMPER_STATE_JUMPING:
		v.y = -JUMPER_JUMPING_SPEED_Y;
		break;
	case JUMPER_STATE_FALLING:
		v.y = JUMPER_JUMPING_SPEED_Y;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Jumper
void CURRENT_CLASS::Interact(Interactable* other) { other->Interact(this); }
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)