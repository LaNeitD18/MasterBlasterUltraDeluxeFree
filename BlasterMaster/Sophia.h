#pragma once
#include "Interactable.h"
#include "GameObject.h"
class Sophia :
	public Player, public Interactable
{
public:
	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF, INTERACTABLE_GROUP);

	virtual void GetBoundingBox(BoundingBox& box);
	virtual void Update();
	virtual void Render();

	Sophia();
	Sophia(float x, float y);
	virtual void SetState(int state);
};

