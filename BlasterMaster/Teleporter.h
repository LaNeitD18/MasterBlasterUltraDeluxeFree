#pragma once
#include "AnimatedGameObject.h"

#define TELEPORTER_BBOX_OFFSET_LEFT		(-12 + 1)
#define TELEPORTER_BBOX_OFFSET_RIGHT	(12 - 1)
#define TELEPORTER_BBOX_OFFSET_TOP		(-16 + 1)
#define TELEPORTER_BBOX_OFFSET_BOTTOM	(16 - 1)

#define PLAYER_ZONE_WIDTH		80
#define PLAYER_ZONE_HEIGHT		80

#define TELEPORTER_SPEED_FLYING_X		1.0f
#define TELEPORTER_SPEED_FLYING_Y		1.0f

enum TeleporterState : int {
	TELEPORTER_STATE_TELEPORT_X	= 100,
	TELEPORTER_STATE_TELEPORT_Y = 101,
	TELEPORTER_STATE_PROTECT	= 102,
	TELEPORTER_STATE_ATTACK		= 103
};

enum TeleporterAni : int {
	TELEPORTER_ANI_TELEPORT = 0,
	TELEPORTER_ANI_PROTECT = 1,
	TELEPORTER_ANI_ATTACK = 2
};

class Teleporter : public Enemy
{
	int timeToTeleport;
	int step; // number of steps to do 1 teleport
	int teleportTurn;

	void SetDirectionX();
	void SetDirectionY();
	void TeleportHorizontally();
	void TeleportVertically();
	void Shoot();
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

