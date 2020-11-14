#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

static D3DCOLOR invulnerableColor[6] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,255), D3DCOLOR_ARGB(0,255,255,255) };

void GameObject::SetState(int state) { this->state = state; }

GameObject::GameObject()
{
	drawArguments.Init();
	v = Point();
	nx = 1;	
}

RESULT GameObject::Init(TextureLibrary * textureLib)
{
	bbox = textureLib->Get(ID_TEX_BBOX);
	return 0;
}

void GameObject::SetPosition(Point pos) { this->pos = pos; }

void GameObject::SetSpeed(Point v) { this->v = v; }

Point GameObject::GetPosition() { return pos; }

Point GameObject::GetSpeed() { return v; }

int GameObject::GetState() { return this->state; }

void GameObject::RenderBoundingBox()
{
	Point pos = drawArguments.GetPosition();
	D3DXVECTOR3 p(pos.x, pos.y, 0);
	RECT rect;

	BoundingBox box = GetBoundingBox();
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)box.r - (int)box.l;
	rect.bottom = (int)box.b - (int)box.t;

	Game::GetInstance()->Draw(pos, bbox, rect, 32);
}

void GameObject::SetAnimationSet(AnimationSet * ani_set) { animationSet = ani_set; }

GameObject::~GameObject()
{

}

void GameObject::TakeDamage(int damage)
{
	if (damage > 0)
		HealthPoint -= damage;
}

int GameObject::GetHP()
{
	return HealthPoint;
}

void GameObject::SetHP(int healthpoint)
{
	HealthPoint = healthpoint;
}

//void AnimatedGameObject::Render()
//{
//	currentAnimation->Render(pos, currentTime, currentFrame);
//	if (!moving)
//		return;
//	currentTime++;
//	if (currentTime >= currentAnimation->GetLoopDuration())
//	{
//		currentTime %= currentAnimation->GetLoopDuration();
//		currentFrame = 0;
//	}
//}

//void AnimatedGameObject::SetAnimationType(int ANI)
//{
//	Animation* trg = animationSet->at(ANI);
//	if (currentAnimation != trg)
//	{
//		currentAnimation = trg;
//		//Long
//		//currentFrame = 0;
//		previousFrame = 0;
//		currentTime = 0;
//	}
//}

void Player::TakeDamage(int damage)
{
	if (invulnerableFrame <= 0 && HealthPoint > 0 && damage > 0) {
		GameObject::TakeDamage(damage);
		invulnerableFrame = DURATION_OF_INVULNERABILITY;
		DebugOut(L"Current HP : %d", HealthPoint);
	}
}

void Player::Update()
{
	if (invulnerableFrame > 0)
	{
		invulnerableFrame--;
	}
	drawArguments.SetColor(invulnerableColor[(invulnerableFrame / SPRITE_DURATION_OF_INVULNERABILTY) % 6]);
}

bool Player::IsPrimaryPlayer() { return true; }
