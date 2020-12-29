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

GateDirection Env_Portal::GetPortalDir()
{
	return dir;
}

int Env_Portal::GetSectionToEnter()
{
	return sectionToEnter;
}

int Env_Portal::GetSectionLocation()
{
	return sectionLocation;
}

Env_Portal::Env_Portal(float x, float y, float width, float height, GateDirection direction, int sectionToEnter, int sectionLocation)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
	this->dir = direction;
	this->sectionToEnter = sectionToEnter;
	this->sectionLocation = sectionLocation;
}

int Env_Dungeon::GetSectionToEnter()
{
	return sectionToEnter;
}

GateDirection Env_Dungeon::GetDungeonDir()
{
	return dir;
}

Env_Dungeon::Env_Dungeon(float x, float y, float width, float height, GateDirection direction, int sectionToEnter)
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

Env_Outdoor::Env_Outdoor(float x, float y, float width, float height, GateDirection direction, int sectionToEnter)
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

GateDirection Env_Outdoor::GetOutDir()
{
	return dir;
}

int Env_Outdoor::GetSectionToEnter()
{
	return sectionToEnter;
}

Env_Ladder::Env_Ladder(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}

Env_Enterboss::Env_Enterboss(float x, float y, float width, float height)
{
	this->width = width;
	this->height = height;
	box.l = x;
	box.t = y;
	box.r = x + width;
	box.b = y + height;
}