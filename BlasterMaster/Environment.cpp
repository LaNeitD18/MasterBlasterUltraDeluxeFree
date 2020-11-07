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

Spike::Spike(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}

void Env_Wall::Interact(Interactable * other) { other->Interact(this); }

void Spike::Interact(Interactable * other) { other->Interact(this); }
