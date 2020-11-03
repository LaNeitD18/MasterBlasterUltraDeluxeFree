#include "StaticObject.h"

StaticObject::StaticObject() {}
StaticObject::StaticObject(float x, float y) {
	pos = Point(x, y);

	Animation* trg = animationSet->at(0);
	currentAnimation = trg;
}

void StaticObject::Update(float dt)
{
}

void StaticObject::GetBoundingBox(BoundingBox& box)
{
	
}

void StaticObject::Update()
{
	
}

void StaticObject::Render()
{
	currentAnimation->Render(Point(0, 0), currentTime, previousFrame);
	previousFrame = 0;
}
