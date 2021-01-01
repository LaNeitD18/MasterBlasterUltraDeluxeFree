#pragma once
#include "AnimatedGameObject.h"

#define OBS_BBOX_OFFSET_RIGHT 8 -1
#define OBS_BBOX_OFFSET_BOTTOM 8 -1
#define OBS_BBOX_OFFSET_TOP -8 +1
#define OBS_BBOX_OFFSET_LEFT -8 +1

#define BREAKABLETREE_ANIMATION_SET_ID 298

class Breakable_Obstacle : public AnimatedGameObject
{
	//int time;
	bool isOut;
public:
	Breakable_Obstacle();
	Breakable_Obstacle(float x, float y, int t);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	// Inherited via AnimatedGameObject
	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();

	BoundingBox GetBoundingBoxJason();

	bool GetIsOut();
	void SetIsOut(bool out);

	int type; // 0: overhead -  1: sideview

};
