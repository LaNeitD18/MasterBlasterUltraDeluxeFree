#pragma once
#include "Interactable.h"
#include "BoundingBox.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Environment {
public:
	virtual BoundingBox GetBoundingBox(BoundingBox& box);
	BoundingBox& box;
};
class Env_Wall : Environment, Interactable
{
public:
	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF, INTERACTABLE_GROUP);
};