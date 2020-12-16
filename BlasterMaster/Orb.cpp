#include "Orb.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "Game.h"
#include "Camera.h"
#include "Player.h"

Orb::Orb() {
	SetState(ORB_STATE_FLYING_TYPE_1);
}

Orb::Orb(float x, float y, int type) {
	this->pos = Point(x, y);
	this->type = type;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));

	initialTime = GetTickCount64();
	v = Point(-ORB_FLY_SPEED_X, 0);
	direction = Point(-1, 0);
	flyVerticallyRange = 0;

	if (type == 1) {
		SetState(ORB_STATE_FLYING_TYPE_1);
	}
	else {
		SetState(ORB_STATE_FLYING_TYPE_2);
		timeToFlyVertically = rand() % 300 + 150;
	}
}

BoundingBox Orb::GetBoundingBox()
{
	float left = pos.x + ORB_BBOX_OFFSET_LEFT;
	float top = pos.y + ORB_BBOX_OFFSET_TOP;
	float right = pos.x + ORB_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + ORB_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void Orb::UpdateOrbType1()
{
	DWORD time = GetTickCount64() - initialTime;

	// chase player
	if (time > 2500) {
		SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
		Point playerPos = Point();
		if (scene != nullptr) {
			playerPos = scene->GetTarget()->GetPosition();
		}

		float module = sqrt(pow(playerPos.x - pos.x, 2) + pow(playerPos.y - pos.y, 2));
		Point distance = (playerPos - pos);

		v.x = distance.x / module;
		v.y = distance.y / module;
	}

	if (isCollided) {
		SetState(ORB_STATE_EXPLODE);
	}

	if (currentTime == 0 && state == ORB_STATE_EXPLODE) {
		manager->RemoveElement(this);
	}
}

void Orb::UpdateOrbType2()
{
	flyVerticallyRange += dx().y;

	if ((v.x < 0 && wallLeft) || (v.x > 0 && wallRight)) {
		direction.x = -direction.x;
		SetState(ORB_STATE_FLYING_TYPE_1);
	}

	// fly vertically
	if (timeToFlyVertically == 0) {
		int randNumber = rand() % 2;
		if (randNumber == 0) { direction.y = 1; }
		else { direction.y = -1; }

		SetState(ORB_STATE_FLYING_Y);
	}

	// range > limit, stop fly vertically, fly horizontally
	if (abs(flyVerticallyRange) > 16) {
		SetState(ORB_STATE_FLYING_TYPE_1);
		flyVerticallyRange = 0;
		timeToFlyVertically = rand() % 300 + 150;
	}
	
	if (timeToFlyVertically >= 0) {
		timeToFlyVertically--;
	}

	if (currentTime == 0 && state == ORB_STATE_FLYING_TYPE_1) {
		SetState(ORB_STATE_FLYING_TYPE_2);
	}

	// reset wall collision
	wallBot = wallLeft = wallRight = wallTop = false;
}

void Orb::Update()
{
	pos += dx();
	Enemy::Update();

	if (type == 1)	UpdateOrbType1();
	else	UpdateOrbType2();
}

void Orb::Render()
{
	if (state == ORB_STATE_FLYING_TYPE_1 || state == ORB_STATE_FLYING_Y) {
		SetAnimationType(ORB_ANI_FLY_1);
	}
	else if (state == ORB_STATE_FLYING_TYPE_2) {
		SetAnimationType(ORB_ANI_FLY_2);
	}
	else if(state == ORB_STATE_EXPLODE) {
		SetAnimationType(ORB_ANI_EXPLODE);
	}

	if (v.x < 0)	isFlipVertical = false;
	else if (v.x > 0)	isFlipVertical = true;

	AnimatedGameObject::Render();
}

void Orb::SetState(int state)
{
	GameObject::SetState(state);

	switch (state) {
	case ORB_STATE_FLYING_TYPE_1:
		v.x = direction.x * ORB_FLY_SPEED_X;
		v.y = 0;
		break;
	case ORB_STATE_FLYING_TYPE_2:
		break;
	case ORB_STATE_FLYING_Y:
		v.x = 0;
		v.y = direction.y * ORB_FLY_SPEED_Y;
		break;
	}
}
