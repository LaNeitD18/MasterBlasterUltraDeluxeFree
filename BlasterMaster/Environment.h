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

class Env_Spike : public Environment
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Env_Spike(float x, float y, float width, float height);
};

class Env_Lava : public Environment
{
public:
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Env_Lava(float x, float y, float width, float height);
};

enum GateDirection {
	LEFT = 0, RIGHT = 1
};

class Env_Portal : public Environment
{
	GateDirection dir;
	int sectionToEnter;
public:
	GateDirection GetPortalDir();
	int GetSectionToEnter();
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Env_Portal(float x, float y, float width, float height, GateDirection direction, int sectionToEnter);
};

class Env_Dungeon : public Environment
{
	GateDirection dir;
	int sectionToEnter;
public:
	GateDirection GetDungeonDir();
	int GetSectionToEnter();
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	Env_Dungeon(float x, float y, float width, float height, GateDirection direction, int sectionToEnter);
};