#include "LinearEyeball.h"
#include "Utils.h"
#include "SceneArea2Overhead.h"
#include "Game.h"
#include "CannonBullet.h"
#include "Sound.h"
#include "Player.h"
#include "GameGlobal.h"

LinearEyeball::LinearEyeball() {

}

LinearEyeball::LinearEyeball(Point pos, Point direction) {
	this->pos = pos;
	drawArguments.SetScale(D3DXVECTOR2(1, 1));
	SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(6));
	SetAnimationType(0);

	this->v = Point(-EYEBALL_SPEED_FLYING_X * direction.x, -EYEBALL_SPEED_FLYING_Y * direction.y);
}

void LinearEyeball::RandomDirection()
{
	int randDirX = rand() % 2;
	if (randDirX)	this->direction.x = 1;
	else			this->direction.x = -1;

	int randDirY = rand() % 2;
	if (randDirY)	this->direction.y = 1;
	else			this->direction.y = -1;
}

BoundingBox LinearEyeball::GetBoundingBox()
{
	float left = pos.x + EYEBALL_BBOX_OFFSET_LEFT;
	float top = pos.y + EYEBALL_BBOX_OFFSET_TOP;
	float right = pos.x + EYEBALL_BBOX_OFFSET_RIGHT;
	float bottom = pos.y + EYEBALL_BBOX_OFFSET_BOTTOM;

	return BoundingBox(left, top, right, bottom);
}

void LinearEyeball::Update()
{
	pos += dx();
	Enemy::Update();

	if (wallBot || wallTop || wallLeft || wallRight) {
		manager->RemoveElement(this);
	}

	wallBot = wallTop = wallLeft = wallRight = false;
}

void LinearEyeball::Render()
{
	AnimatedGameObject::Render();

	//RenderBoundingBox();
}

void LinearEyeball::SetState(int state)
{
	GameObject::SetState(state);
}