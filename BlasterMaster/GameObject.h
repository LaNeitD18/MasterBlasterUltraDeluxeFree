#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "useful_stuff.h"
#include "Manager.h"
#include "Sprites.h"
#include "Animations.h"
#include "Textures.h"
#include "BoundingBox.h"
#include "DrawArguments.h"
#include "Camera.h"
#include "Interactable.h"

using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class GameObject : public Interactable, public Managed<GameObject>
{
public:
	Point pos;

	Point dx() { return v * dt; }

	Point v;

	int nx;	 

	int state;

	DWORD dt = 1;

	float scaleIndex = 1;

	AnimationSet* animationSet;
	LPDIRECT3DTEXTURE9 bbox;

	DrawArguments drawArguments;

public: 
	void SetPosition(Point pos);
	void SetSpeed(Point v);

	Point GetPosition();
	//Point GetPosition() { return this->drawArguments.GetPosition(); }

	Point GetSpeed();

	int GetState();

	void RenderBoundingBox();

	virtual void SetAnimationSet(AnimationSet* ani_set);
	
	virtual RESULT Init(TextureLibrary* textureLib);

	virtual BoundingBox GetBoundingBox() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void SetState(int state);

	GameObject();
	virtual ~GameObject();

	// mark if collide with wall in different pos
	bool wallTop;
	bool wallBot;
	bool wallLeft;
	bool wallRight;

	// used for interact with damage
	int HealthPoint;
	virtual void TakeDamage(int damage);

	int GetHP();
	void SetHP(int healthpoint);
};
