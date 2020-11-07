#include "Interactable.h"
#include "Sophia.h"
#include "Environment.h"
#include "Worm.h"

Interactable::Interactable()
{
}


Interactable::~Interactable()
{
}

#define DAMAGE_OF_SPIKE 25

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
#pragma endregion

#undef DAMAGE_OF_SPIKE
