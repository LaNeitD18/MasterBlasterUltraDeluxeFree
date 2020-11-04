#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "useful_stuff.h"
#include "Sprites.h"
#include "Animations.h"
#include "Textures.h"
#include "BoundingBox.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject; 

class GameObject
{
public:
	Point pos;

	Point dx() { return v * dt; }

	Point v;

	int nx;	 

	int state;

	DWORD dt = 33; 

	AnimationSet* animationSet;
	LPDIRECT3DTEXTURE9 bbox;
public: 
	void SetPosition(Point pos) { this->pos = pos; }
	void SetSpeed(Point v) { this->v = v; }
	Point GetPosition() { return this->pos; }
	Point GetSpeed() { return this->v; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(AnimationSet* ani_set) { animationSet = ani_set; }
	
	virtual RESULT Init(TextureLibrary* textureLib);

	virtual void GetBoundingBox(BoundingBox&) = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	GameObject();
	~GameObject();
};

class AnimatedGameObject : public GameObject
{
protected:
	int previousFrame;
	int currentTime;
	Animation* currentAnimation;
	bool moving = true;
public:
	virtual void Render();
	void SetAnimationType(int ANI);
};

class Enemy : public AnimatedGameObject 
{};

class Player : public AnimatedGameObject
{};