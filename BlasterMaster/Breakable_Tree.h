#pragma once
#include "AnimatedGameObject.h"

#define BREAKABLETREE_BBOX_OFFSET_RIGHT 8 -1
#define BREAKABLETREE_BBOX_OFFSET_BOTTOM 8 -1
#define BREAKABLETREE_BBOX_OFFSET_TOP -8 +1
#define BREAKABLETREE_BBOX_OFFSET_LEFT -8 +1

#define BREAKABLETREE_ANIMATION_SET_ID 298

class Breakable_Tree : public AnimatedGameObject
{
	//int time;
	bool isOut;
public:
	Breakable_Tree();
	Breakable_Tree(float x, float y);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

	BoundingBox GetBoundingBoxJason();

	bool GetIsOut();
	void SetIsOut(bool out);

};
