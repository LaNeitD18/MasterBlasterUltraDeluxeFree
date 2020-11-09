#include "Environment.h"

Environment::Environment()
{
	box.l = 0;
	box.t = 0;
	box.r = 0;
	box.b = 0;
}

Environment::~Environment()
{
}

BoundingBox Environment::GetBoundingBox()
{
	return this->box;
}

float Environment::GetWidth()
{
	return width;
}

float Environment::GetHeight()
{
	return height;
}

void Environment::SetWidth(float x)
{
	width = x;
}

void Environment::SetHeight(float x)
{
	height = x;
}

Point Environment::GetPosition()
{
	return pos;
}

Env_Wall::Env_Wall(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}

Env_Spike::Env_Spike(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}

Env_Lava::Env_Lava(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}

PortalDirection Env_Portal::GetPortalDir()
{
	return dir;
}

int Env_Portal::GetSectionToEnter()
{
	return sectionToEnter;
}

Env_Portal::Env_Portal(float x, float y, float width, float height, PortalDirection direction, int sectionToEnter)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
	this->dir = direction;
	this->sectionToEnter = sectionToEnter;
}

void Env_Wall::Interact(Interactable * other) { other->Interact(this); }

void Env_Spike::Interact(Interactable * other) { other->Interact(this); }

void Env_Lava::Interact(Interactable * other) { other->Interact(this); }
#include "InteractableGroupInclude.h"
#define CURRENT_CLASS Env_Wall
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS
#define CURRENT_CLASS Env_Spike
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS
//*
#define CURRENT_CLASS Env_Lava
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS
//*/
#define CURRENT_CLASS Env_Portal
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS
