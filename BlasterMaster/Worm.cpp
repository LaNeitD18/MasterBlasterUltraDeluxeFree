#include "Worm.h"
#include "Utils.h"
#include "GameGlobal.h"
#include "Game.h"
#include "SceneArea2SideView.h"

Worm::Worm() {
	SetState(WORM_STATE_FALLING);
}

Worm::Worm(float x, float y)
{
	SetState(WORM_STATE_FALLING);
	pos = Point(x, y);
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	isFlipVertical = true;
	direction = Point(-1, 1);
}

BoundingBox Worm::GetBoundingBox()
{
	float left = pos.x + WORM_BBOX_OFFSET_LEFT;
	float top = pos.y + WORM_BBOX_OFFSET_TOP;
	float right = pos.x + WORM_BBOX_OFFSET_RIGHT;
	float bottom;
	if (state == WORM_STATE_DIE)
		bottom = pos.y + WORM_BBOX_HEIGHT_DIE;
	else
		bottom = pos.y + WORM_BBOX_OFFSET_BOTTOM;
	return BoundingBox(left, top, right, bottom);
}

void Worm::Fall() 
{
	// worm can only have v.x when falling if it falls from the top (1/3 screen height)
	if (!wallLeft && this->drawArguments.GetPosition().y < GameGlobal::GetHeight()/3)
		v.x = -WORM_FALLING_SPEED_X;
	if (wallBot) {
		jumpRange = 0;
		SetState(WORM_STATE_WALKING);
	}
}

void Worm::Walk()
{
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Point playerPos = scene->GetTarget()->GetPosition();
	float speed = WORM_WALKING_SPEED;
	//DebugOut(L"%fPX , %fPY \n", playerPos.x, playerPos.y);

	if (wallLeft || wallRight) {
		//v.x = -v.x;
		SetState(WORM_STATE_JUMPING);
	}
	if (!wallBot) {
		SetState(WORM_STATE_FALLING);
	}
	if (pos.x < playerPos.x - 16) {
		direction.x = 1;
		v.x = speed * direction.x;
	}
	if (pos.x > playerPos.x + 16) {
		direction.x = -1;
		v.x = speed * direction.x;
	}
}

void Worm::Jump() 
{
	if (jumpRange > 16) {
		if (!wallLeft && !wallRight) {
			SetState(WORM_STATE_WALKING);
		} else{
			SetState(WORM_STATE_FALLING);
		}
	}
	
}

void Worm::Update()
{
	pos += dx();	
	Enemy::Update();

	if (state == WORM_STATE_FALLING) {
		Fall();
	}
	else if (state == WORM_STATE_WALKING) {
		Walk();
	}
	else if (state == WORM_STATE_JUMPING) {
		jumpRange += abs(dx().y);
		Jump();
	}
	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Worm::Render()
{
	if (state == WORM_STATE_DIE) {
		SetAnimationType(WORM_ANI_DIE);
	}
	else if (state == WORM_STATE_FALLING) {
		SetAnimationType(WORM_ANI_IDLING);
	}
	else {
		SetAnimationType(WORM_ANI_WALKING_RIGHT);
		if (v.x > 0) {
			isFlipVertical = false;
		}
		else if (v.x < 0) {
			isFlipVertical = true;
		}
	}
	DebugOut(L"%fvx \n", v.x);
	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void Worm::SetState(int state)
{
	GameObject::SetState(state);
	float speed = WORM_WALKING_SPEED;
	switch (state)
	{
	case WORM_STATE_DIE:
		pos.y += WORM_BBOX_HEIGHT - WORM_BBOX_HEIGHT_DIE + 1;
		v.x = 0;
		v.y = 0;
		break;
	case WORM_STATE_WALKING:
		v.x = speed * direction.x;
		v.y = 0;
		break;
	case WORM_STATE_FALLING:
		v.y = WORM_FALLING_SPEED_Y;
		break;
	case WORM_STATE_JUMPING:
		v.y = -WORM_JUMPING_SPEED_Y;
		v.x = 0;
		break;
	}
}

#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Worm
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS