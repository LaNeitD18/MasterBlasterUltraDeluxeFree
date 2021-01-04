#include "Sound.h"
#include "Player.h"
#include "Utils.h"

static D3DCOLOR invulnerableColor[6] = { D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,0,255,255),D3DCOLOR_ARGB(255,255,255,255),D3DCOLOR_ARGB(255,255,0,255),D3DCOLOR_ARGB(255,255,255,255), D3DCOLOR_ARGB(0,255,255,255) };

void Player::TakeDamage(int damage)
{
	if (invulnerableFrame <= 0 && HealthPoint > 0 && damage > 0) {
		GameObject::TakeDamage(damage);
		invulnerableFrame = DURATION_OF_INVULNERABILITY;
		Sound::getInstance()->play("lava", false, 1);
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

void Player::AddElement(Bullet * bullet)
{
	bullets.insert(bullet);
}

void Player::RemoveElement(Bullet * bullet)
{
	bullets.erase(bullet);
}