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
#define DURATION_OF_INVULNERABILITY 200
#define SPRITE_DURATION_OF_INVULNERABILTY 10

class GameObject : public Interactable, public Managed<GameObject>
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

#define INPUT_JUMP				'X'
#define INPUT_LEFT				VK_LEFT
#define INPUT_RIGHT				VK_RIGHT
#define INPUT_UP				VK_UP
#define INPUT_DOWN				VK_DOWN
#define INPUT_LEAVE_VEHICLE		'C'
#define INPUT_SHOOT				'Z'

class Player : public GameObject
{
public:
	int invulnerableFrame;
	virtual void TakeDamage(int damage);
	virtual void Update() override;

	virtual bool IsPrimaryPlayer();

	virtual ~Player() {}
	Player() {}
};
