#include "Interactable.h"
#include "Sophia.h"
#include "Environment.h"
#include "Worm.h"
#include "Floater.h"
#include "Dome.h"
#include "Jumper.h"
#include "Utils.h"
#include "SceneArea2SideView.h"
#include "SceneArea2Overhead.h"
#include "JasonSideView.h"
#include "JasonOverhead.h"

Interactable::Interactable()
{
}


Interactable::~Interactable()
{
}

#define DAMAGE_OF_SPIKE 25
#define DAMAGE_OF_LAVA 25

void Interactable::Interact(Player * player, Env_Wall * wall) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (playerBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(playerBox.r, wallBox.r) - max(playerBox.l, wallBox.l);
		float overlapAreaY = min(playerBox.b, wallBox.b) - max(playerBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (playerBox.GetCenter().y > wallBox.GetCenter().y) {
				player->wallTop = true;
				// Snap top (player pushed down)
				Point pos = player->GetPosition();
				pos.y -= playerBox.t - wallBox.b;
				player->SetPosition(pos);
			}
			else
			{
				player->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = player->GetPosition();
				pos.y += wallBox.t - playerBox.b;
				player->SetPosition(pos);
			}
		}
		else
		{
			if (playerBox.GetCenter().x < wallBox.GetCenter().x) {
				player->wallRight = true;
				// Snap right (player to left)
				Point pos = player->GetPosition();
				pos.x -= playerBox.r - wallBox.l;
				player->SetPosition(pos);
			}
			else
			{
				player->wallLeft = true;
				// Snap left (player to right)
				Point pos = player->GetPosition();
				pos.x += wallBox.r - playerBox.l;
				player->SetPosition(pos);
			}
		}
	}
}

void Interactable::Interact(Player * player, Env_Spike * spike) {
	// implement interact with spike (take damage)
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox spikeBox = spike->GetBoundingBox();
	if (playerBox.IsOverlap(spikeBox)) {
		player->TakeDamage(DAMAGE_OF_SPIKE); // define later
	}
}

void Interactable::Interact(Player* player, Env_Lava* lava) {
	// implement interact with lava (take damage)
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox lavaBox = lava->GetBoundingBox();
	if (playerBox.IsOverlap(lavaBox)) {
		player->TakeDamage(DAMAGE_OF_LAVA);
	}
}

void Interactable::Interact(Player* player, Env_Portal* portal) {
	// implement interact with lava (take damage)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox portalBox = portal->GetBoundingBox();
	if (playerBox.IsOverlap(portalBox)) {
		GateDirection portalDirection = portal->GetPortalDir();
		if (((input[VK_RIGHT] & KEY_STATE_DOWN) && portalDirection == RIGHT) || ((input[VK_LEFT] & KEY_STATE_DOWN) && portalDirection == LEFT)) {
			BoundingBox limitArea = SceneArea2SideView::cameraLimitAreaOfSection[portal->GetSectionToEnter()];
			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
			Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
			if (portalDirection == RIGHT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(1);
			}
			else {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(0);
			}
			Camera::GetInstance()->SetCameraLimitarea(limitArea);
			DebugOut(L"%d\n", portalDirection);
		}
	}
}

void Interactable::Interact(Player* player, Env_Dungeon* dungeon) {
	// implement interact with lava (take damage)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox dungeonBox = dungeon->GetBoundingBox();
	if (playerBox.IsOverlap(dungeonBox)) {
		GateDirection gateDirection = dungeon->GetDungeonDir();
		if ((input[VK_RIGHT] && gateDirection == RIGHT) || (input[VK_LEFT] && gateDirection == LEFT)) {
			BoundingBox limitArea = SceneArea2Overhead::cameraLimitAreaOfSection[dungeon->GetSectionToEnter()];
			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
			//Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
			Game::GetInstance()->Init(L"Resources/scene.txt", 3);
			Camera::GetInstance()->SetCameraLimitarea(limitArea);
		}
	}
}


#pragma region Tien
void Interactable::Interact(Worm* worm, Env_Wall* wall) {
	BoundingBox wormBox = worm->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (wormBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(wormBox.r, wallBox.r) - max(wormBox.l, wallBox.l);
		float overlapAreaY = min(wormBox.b, wallBox.b) - max(wormBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (wormBox.GetCenter().y > wallBox.GetCenter().y) {
				worm->wallTop = true;
				// Snap top (player pushed down)
				Point pos = worm->GetPosition();
				pos.y -= wormBox.t - wallBox.b;
				worm->SetPosition(pos);
			}
			else
			{
				worm->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = worm->GetPosition();
				pos.y += wallBox.t - wormBox.b;
				worm->SetPosition(pos);
			}
		}
		else
		{
			if (wormBox.GetCenter().x < wallBox.GetCenter().x) {
				worm->wallRight = true;
				// Snap right (player to left)
				Point pos = worm->GetPosition();
				pos.x -= wormBox.r - wallBox.l;
				worm->SetPosition(pos);
			}
			else
			{
				worm->wallLeft = true;
				// Snap left (player to right)
				Point pos = worm->GetPosition();
				pos.x += wallBox.r - wormBox.l;
				worm->SetPosition(pos);
			}
		}
	}
}

void Interactable::Interact(Floater* floater, Env_Wall* wall) {
	BoundingBox floaterBox = floater->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (floaterBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(floaterBox.r, wallBox.r) - max(floaterBox.l, wallBox.l);
		float overlapAreaY = min(floaterBox.b, wallBox.b) - max(floaterBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (floaterBox.GetCenter().y > wallBox.GetCenter().y) {
				floater->wallTop = true;
				// Snap top (player pushed down)
				Point pos = floater->GetPosition();
				pos.y -= floaterBox.t - wallBox.b;
				floater->SetPosition(pos);
			}
			else
			{
				floater->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = floater->GetPosition();
				pos.y += wallBox.t - floaterBox.b;
				floater->SetPosition(pos);
			}
		}
		else
		{
			if (floaterBox.GetCenter().x < wallBox.GetCenter().x) {
				floater->wallRight = true;
				// Snap right (player to left)
				Point pos = floater->GetPosition();
				pos.x -= floaterBox.r - wallBox.l;
				floater->SetPosition(pos);
			}
			else
			{
				floater->wallLeft = true;
				// Snap left (player to right)
				Point pos = floater->GetPosition();
				pos.x += wallBox.r - floaterBox.l;
				floater->SetPosition(pos);
			}
		}
	}
}

void Interactable::Interact(Dome* dome, Env_Wall* wall) {
	BoundingBox domeBox = dome->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (domeBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(domeBox.r, wallBox.r) - max(domeBox.l, wallBox.l);
		float overlapAreaY = min(domeBox.b, wallBox.b) - max(domeBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (domeBox.GetCenter().y > wallBox.GetCenter().y) {
				dome->wallTop = true;
				// Snap top (player pushed down)
				Point pos = dome->GetPosition();
				pos.y -= domeBox.t - wallBox.b;
				dome->SetPosition(pos);
			}
			else
			{
				dome->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = dome->GetPosition();
				pos.y += wallBox.t - domeBox.b;
				dome->SetPosition(pos);
			}
		}
		else
		{
			if (domeBox.GetCenter().x < wallBox.GetCenter().x) {
				dome->wallRight = true;
				// Snap right (player to left)
				Point pos = dome->GetPosition();
				pos.x -= domeBox.r - wallBox.l;
				dome->SetPosition(pos);
			}
			else
			{
				dome->wallLeft = true;
				// Snap left (player to right)
				Point pos = dome->GetPosition();
				pos.x += wallBox.r - domeBox.l;
				dome->SetPosition(pos);
			}
		}
	}
}

void Interactable::Interact(Jumper* jumper, Env_Wall* wall) {
	BoundingBox jumperBox = jumper->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (jumperBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(jumperBox.r, wallBox.r) - max(jumperBox.l, wallBox.l);
		float overlapAreaY = min(jumperBox.b, wallBox.b) - max(jumperBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (jumperBox.GetCenter().y > wallBox.GetCenter().y) {
				jumper->wallTop = true;
				// Snap top (player pushed down)
				Point pos = jumper->GetPosition();
				pos.y -= jumperBox.t - wallBox.b;
				jumper->SetPosition(pos);
			}
			else
			{
				jumper->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = jumper->GetPosition();
				pos.y += wallBox.t - jumperBox.b;
				jumper->SetPosition(pos);
			}
		}
		else
		{
			if (jumperBox.GetCenter().x < wallBox.GetCenter().x) {
				jumper->wallRight = true;
				// Snap right (player to left)
				Point pos = jumper->GetPosition();
				pos.x -= jumperBox.r - wallBox.l;
				jumper->SetPosition(pos);
			}
			else
			{
				jumper->wallLeft = true;
				// Snap left (player to right)
				Point pos = jumper->GetPosition();
				pos.x += wallBox.r - jumperBox.l;
				jumper->SetPosition(pos);
			}
		}
	}
}

// JasonOverhead
void Interactable::Interact(JasonOverhead* player, Env_Wall* wall)
{
	Interactable::Interact((Player*)player, wall);
	if (player->wallBot) {

	}
}
#pragma endregion

#pragma region Long
void Interactable::Interact(JasonSideView * player, Env_Wall * wall)
{
	Interactable::Interact((Player*)player, wall);
	if (player->wallBot) {
		Point v = player->v;
		if (v.y > JASON_JUMP_SPEED + JASON_GRAVITY * 3) {
			float damage = v.y / JASON_JUMP_SPEED;
			damage *= damage;
			damage -= 1;
			damage *= damage;
			damage *= JASON_MAX_HEALTH;
			player->TakeDamage(round(damage));
		}
	}
}
void Interactable::Interact(Sophia* sophia, JasonSideView * jason)
{
	if (jason->GetBoundingBox().
		IsInsideBox(sophia->GetBoundingBox()))
		jason->sophia = sophia;
}
#pragma endregion

#undef DAMAGE_OF_SPIKE
