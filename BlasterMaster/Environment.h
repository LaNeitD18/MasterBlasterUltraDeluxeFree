#pragma once
#include "Interactable.h"
#include "BoundingBox.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

class Environment : public Interactable {
public:
	Environment();
	~Environment();
	virtual BoundingBox GetBoundingBox();
	float GetWidth();
	float GetHeight();
	void SetWidth(float x);
	void SetHeight(float x);
	Point GetPosition();
	BoundingBox box;
protected:
	float width;
	float height;
	Point pos; // top left point
};
class Env_Wall : public Environment
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Env_Wall(float x, float y, float width, float height);
};

class Spike : public Environment
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Spike(float x, float y, float width, float height);
};