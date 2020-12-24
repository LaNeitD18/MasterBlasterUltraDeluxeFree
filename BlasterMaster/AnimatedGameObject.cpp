#include "AnimatedGameObject.h"
#include "Utils.h"
#include "ItemPower.h"
#include "ItemHover.h"
#include "Boss.h"

static D3DCOLOR invulnerableColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(0,255,255,255) };
static D3DCOLOR damageBossColor[2] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255, 50, 50, 50) };

void AnimatedGameObject::Render()
{
	// set to world coordinate
	Point cameraOffset = Camera::GetInstance()->GetPosition();
	cameraOffset.x -= Camera::GetInstance()->GetWidth() / 2;
	cameraOffset.y -= Camera::GetInstance()->GetHeight() / 2;
	drawArguments.SetPosition(pos - cameraOffset);

	//drawArguments.SetPosition(pos);
	drawArguments.FlipVertical(isFlipVertical);
	drawArguments.FlipHorizontal(isFlipHorizontal);
	drawArguments.SetRotation(rotation);

	currentAnimation->Render(currentTime, previousFrame, drawArguments);
	if (!moving)
		return;
	currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		currentTime %= currentAnimation->GetLoopDuration();
		previousFrame = 0;
	}
}

void AnimatedGameObject::SetAnimationType(int ANI)
{
	Animation* trg = animationSet->at(ANI);
	if (currentAnimation != trg)
	{
		currentAnimation = trg;
		previousFrame = 0;
		currentTime = 0;
	}
}

#define RATE_DISPLAY_POWER_ITEM 75
#define RATE_DISPLAY_HOVER_ITEM 5

void Enemy::TakeDamage(int damage)
{
	if (damage <= 0)
		return;
	HealthPoint -= damage;
	damageFrame = DURATION_OF_DAMAGE_FLASH;
	if(dynamic_cast<Boss*>(this) != NULL){
		return;
	}
	if (HealthPoint <= 0) {
		// get position of obj
		Point pos = this->GetPosition();
		manager->RemoveElement(this);
		// TODO: Generate HP items 75 percent
		int random = rand() % 100 + 1;
		//*
		if (random <= RATE_DISPLAY_POWER_ITEM) {
			//displayMessage("power");
			ItemPower* item_power = new ItemPower(pos);
			item_power->SetManager(manager);
			manager->AddElement(item_power);
		}
		//*/
		else if (random <= RATE_DISPLAY_POWER_ITEM + RATE_DISPLAY_HOVER_ITEM) {
			ItemHover* item_hover = new ItemHover(pos);
			item_hover->SetManager(manager);
			manager->AddElement(item_hover);
		}
		//*/
	}
}


void Enemy::Update()
{
	if (damageFrame > 0)
	{
		damageFrame--;
	}
	drawArguments.SetColor(invulnerableColor[(damageFrame / ENEMY_SPRITE_DURATION_OF_DAMAGE_FLASH) % 2]);
}

