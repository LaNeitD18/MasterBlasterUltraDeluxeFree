#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "useful_stuff.h"
#include "Sprites.h"
#include "Animations.h"
#include "Textures.h"
#include "BoundingBox.h"
#include "DrawArguments.h"
#include "Camera.h"
#include "Interactable.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box
#define DURATION_OF_INVULNERABILITY 60
#define SPRITE_DURATION_OF_INVULNERABILTY 10

class GameObject : public Interactable
{
public:
	Point pos;

	Point dx() { return v * dt; }

	Point v;

	int nx;	 

	int state;

	DWORD dt = 33; 

	float scaleIndex = 0.3;

	AnimationSet* animationSet;
	LPDIRECT3DTEXTURE9 bbox;

	DrawArguments drawArguments;

public: 
	void SetPosition(Point pos) { this->pos = pos; }
	void SetSpeed(Point v) { this->v = v; }

	Point GetPosition() { return pos; }
	//Point GetPosition() { return this->drawArguments.GetPosition(); }

	Point GetSpeed() { return v; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	virtual void SetAnimationSet(AnimationSet* ani_set) { animationSet = ani_set; }
	
	virtual RESULT Init(TextureLibrary* textureLib);

	virtual BoundingBox GetBoundingBox() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	GameObject();
	~GameObject();

	// mark if collide with wall in different pos
	bool wallTop;
	bool wallBot;
	bool wallLeft;
	bool wallRight;

	// used for interact with damage
	int HealthPoint;
	virtual void TakeDamage(int damage);
};

class Player : public GameObject
{
public:
	int invulnerableFrame;
	virtual void TakeDamage(int damage);
	virtual void Update();
};
