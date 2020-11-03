#pragma once
#include "GameObject.h"
#include "Game.h"
class StaticObject : public GameObject
{
	int spriteID;
	Sprite* sprite;
	Animation* currentAnimation;
	int previousFrame;
	int currentTime;

	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

public:
	StaticObject();
	StaticObject(float x, float y);

	void SetSpriteID(int spriteID) { this->spriteID = spriteID; }
	RECT GetCollisionBox() { return RECT(); }
	void Update(float dt);
	Sprite* GetSprite() { return sprite; }
};

