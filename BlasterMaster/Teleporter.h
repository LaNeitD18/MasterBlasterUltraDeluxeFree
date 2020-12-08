#pragma once
#include "AnimatedGameObject.h"

#define TELEPORTER_BBOX_OFFSET_LEFT		(-12 + 1)
#define TELEPORTER_BBOX_OFFSET_RIGHT	(12 - 1)
#define TELEPORTER_BBOX_OFFSET_TOP		(-16 + 1)
#define TELEPORTER_BBOX_OFFSET_BOTTOM	(16 - 1)

#define TELEPORTER_SPEED_FLYING_X		1.0f
#define TELEPORTER_SPEED_FLYING_Y		1.0f

enum TeleporterState : int {
	TELEPORTER_STATE_TELEPORT	= 100,
	TELEPORTER_STATE_PROTECT	= 101
};

enum TeleporterAni : int {
	TELEPORTER_ANI_TELEPORT = 0,
	TELEPORTER_ANI_PROTECT = 1
};

class Teleporter : public Enemy
{
public:
	Teleporter();
	Teleporter(float x, float y);
	virtual void SetState(int state);

	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);

	virtual BoundingBox GetBoundingBox();
	virtual void Update();
	virtual void Render();
};

