#include "Interactable.h"
#include "InteractableGroupInclude.h"
#include "SceneArea2SideView.h"
#include "SceneArea2Overhead.h"
#include "Game.h"
#include "Utils.h"

Interactable::Interactable()
{
}


Interactable::~Interactable()
{
}

#define DAMAGE_OF_SPIKE 15
#define DAMAGE_OF_ENEMY 10
#define DAMAGE_OF_LAVA 15

void Interactable::Interact(Player * player, Env_Wall * wall) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	//*
	bool top, left, right, bottom;
	Point move = player->dx();
	top = left = right = bottom = false;
	double offsetTime = wallBox.SweptAABB(playerBox, move, top, left, bottom, right);

	player->wallTop |= top;
	player->wallRight |= right;
	player->wallLeft |= left;
	player->wallBot |= bottom;

	if (offsetTime >= 0.0 && offsetTime <= 1.0)
		move = move - move * offsetTime;
	else
		return;

	if (top || bottom) {
		Point v = player->GetSpeed();
		v.y -= move.y;
		player->SetSpeed(v);
	}
	if (left || right) {
		Point v = player->GetSpeed();
		v.x -= move.x;
		player->SetSpeed(v);
	}
	/*/
	playerBox.Move(player->dx());
	Point playerBoxCenter = playerBox.GetCenter();
	if (playerBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(playerBox.r, wallBox.r) - max(playerBox.l, wallBox.l);
		float overlapAreaY = min(playerBox.b, wallBox.b) - max(playerBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (playerBoxCenter.y > wallBox.GetCenter().y) {
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
			if (playerBoxCenter.x < wallBox.GetCenter().x) {
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
	//*/
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
	if (/*playerBox.IsOverlap(portalBox) &&*/ portalBox.IsInsideBox(playerBox.GetCenter())) {
		GateDirection portalDirection = portal->GetPortalDir();
		if (((input[VK_RIGHT] & KEY_STATE_DOWN) && portalDirection == RIGHT) || ((input[VK_LEFT] & KEY_STATE_DOWN) && portalDirection == LEFT) || ((input[VK_UP] & KEY_STATE_DOWN) && portalDirection == TOP) || ((input[VK_DOWN] & KEY_STATE_DOWN) && portalDirection == BOTTOM)) {
			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
			Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
			if (portalDirection == RIGHT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(1);
			}
			else if (portalDirection == LEFT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(0);
			}
			else if (portalDirection == TOP) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(2);
			}
			else {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(3);
			}
			int sectionToEnter = portal->GetSectionToEnter();
			if (sectionToEnter == -1) {
				BoundingBox currentCameraLimit = Camera::GetInstance()->GetBound();
				if (portalDirection == RIGHT) {
					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l + 256, currentCameraLimit.t, currentCameraLimit.r + 256, currentCameraLimit.b);
				}
				else if (portalDirection == LEFT) {
					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l - 256, currentCameraLimit.t, currentCameraLimit.r - 256, currentCameraLimit.b);
				}
				else if (portalDirection == TOP) {
					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l, currentCameraLimit.t - 256, currentCameraLimit.r, currentCameraLimit.b - 256);
				}
				else {
					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l, currentCameraLimit.t + 256, currentCameraLimit.r, currentCameraLimit.b + 256);
				}
			}
			else {
				BoundingBox limitArea = NULL;
				if (dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene())) {
					limitArea = SceneArea2SideView::cameraLimitAreaOfSection[sectionToEnter];
				}
				else if (dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene())) {
					limitArea = SceneArea2Overhead::cameraLimitAreaOfSection[sectionToEnter];
				}
				Camera::GetInstance()->SetCameraLimitarea(limitArea);
			}
			
			DebugOut(L"%d\n", portalDirection);
		}
	}
}

void Interactable::Interact(Player* player, Env_Dungeon* dungeon) {
	// implement interact with lava (take damage)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox dungeonBox = dungeon->GetBoundingBox();
	JasonSideView* jasonPlay = dynamic_cast<JasonSideView*>(player);
	bool isJasonPlay = jasonPlay != NULL;
	if (playerBox.IsOverlap(dungeonBox) && isJasonPlay) {
		if (input[VK_DOWN] & KEY_STATE_DOWN) {
			BoundingBox limitArea = SceneArea2Overhead::cameraLimitAreaOfSection[dungeon->GetSectionToEnter()];
			Point startPoint = SceneArea2Overhead::startPointInSection[dungeon->GetSectionToEnter()];
			//Game::GetInstance()->GetCurrentScene()->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 3);
			SceneArea2Overhead* scene = dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene());
			for (auto x : scene->GetObjects()) {
				JasonOverhead* current_player = dynamic_cast<JasonOverhead*>(x);
				if (current_player != NULL &&
					current_player->IsPrimaryPlayer()) {
					scene->SetTarget(current_player);
					break;
				}
			}
			scene->GetTarget()->SetPosition(startPoint);
			scene->GetCamera()->SetCameraLimitarea(limitArea);
			//Camera::GetInstance()->SetCameraLimitarea(limitArea);
			Sophia* sophia = dynamic_cast<Sophia*>(jasonPlay->sophia); // sophia null
			GameGlobal::SetLastPositionSophia(sophia->GetPosition());
		}
	}
}

void Interactable::Interact(Player* player, Env_Outdoor* outdoor) {
	// implement interact with lava (take damage)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox outdoorBox = outdoor->GetBoundingBox();
	if (outdoorBox.IsInsideBox(playerBox.GetCenter())) {
		GateDirection doorDirection = outdoor->GetOutDir();
		if (((input[VK_RIGHT] & KEY_STATE_DOWN) && doorDirection == RIGHT) || ((input[VK_LEFT] & KEY_STATE_DOWN) && doorDirection == LEFT) || ((input[VK_UP] & KEY_STATE_DOWN) && doorDirection == TOP) || ((input[VK_DOWN] & KEY_STATE_DOWN) && doorDirection == BOTTOM)) {
			BoundingBox limitArea = SceneArea2SideView::cameraLimitAreaOfSection[outdoor->GetSectionToEnter()];
			Point startPoint = SceneArea2SideView::startPointInSection[outdoor->GetSectionToEnter()];
			//Game::GetInstance()->GetCurrentScene()->Release();
			Game::GetInstance()->Init(L"Resources/scene.txt", 2);
			SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
			Sophia* current_player = NULL;
			for (auto x : scene->GetObjects()) {
				current_player = dynamic_cast<Sophia*>(x);
				if (current_player != NULL &&
					current_player->IsPrimaryPlayer()) {
					scene->SetTarget(current_player);
					//current_player->SetPosition(startPoint);
					current_player->SetPosition(GameGlobal::GetLastPositionSophia());
					break;
				}
			}
			JasonSideView* jason = new JasonSideView(startPoint.x, startPoint.y);
			current_player->jason = jason;
			jason->sophia = current_player;
			jason->SetAnimationSet(GameGlobal::GetAnimationSetLibrary()->Get(JASON_SIDEVIEW_ANIMATION_SET_NUMBER));
			jason->SetManager(current_player->GetManager());
			jason->v.x = 0;
			jason->v.y = -JASON_ENTER_VEHICLE_DISAPPEAR_SPEED;
			current_player->GetManager()->AddElement(jason);
			current_player->SetState(SOPHIA_STATE_LEFT_VEHICLE);
			scene->SetTarget(jason);
			//scene->GetTarget()->SetPosition(startPoint);
			scene->GetCamera()->SetCameraLimitarea(limitArea);
			//Camera::GetInstance()->SetCameraLimitarea(limitArea);
			
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

void Interactable::Interact(Player* player, Enemy* enemy) {
	// implement interact between player and enemy (take damage)
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox enemyBox = enemy->GetBoundingBox();
	if (playerBox.IsOverlap(enemyBox)) {
		player->TakeDamage(DAMAGE_OF_ENEMY);
	}
}
#pragma endregion

#pragma region Long
void Interactable::Interact(JasonSideView * player, Env_Wall * wall)
{
	//*
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	bool top, left, right, bottom;
	Point move = player->dx();
	top = left = right = bottom = false;
	double offsetTime = wallBox.SweptAABB(playerBox, move, top, left, bottom, right);

	player->wallTop |= top;
	player->wallRight |= right;
	player->wallLeft |= left;
	player->wallBot |= bottom;

	if (offsetTime >= 0.0 && offsetTime <= 1.0)
		move = move - move * offsetTime;
	else
		return;

	if (top || bottom) {
		Point v = player->GetSpeed();
		if (bottom && v.y > JASON_JUMP_SPEED + JASON_GRAVITY) {
			float damage = v.y / JASON_JUMP_SPEED;
			damage = (damage * damage - 1.31) / 0.7;
			damage *= JASON_MAX_HEALTH;
			player->TakeDamage(round(damage));
		}
		v.y -= move.y;
		player->SetSpeed(v);
	}
	if (left || right) {
		Point v = player->GetSpeed();
		v.x -= move.x;
		player->SetSpeed(v);
	}
	/*/
	Interactable::Interact((Player*)player, wall);
	if (player->wallBot) {
		Point v = player->v;
		if (v.y > JASON_JUMP_SPEED + JASON_GRAVITY) {
			float damage = v.y / JASON_JUMP_SPEED;
			damage = (damage * damage - 1.24) / 1.37;
			damage *= JASON_MAX_HEALTH;
			player->TakeDamage(round(damage));
		}
	}
	//*/
}
void Interactable::Interact(Sophia* sophia, JasonSideView * jason)
{
	if (jason->GetBoundingBox().
		IsInsideBox(sophia->GetBoundingBox()))
		//jason->sophia = sophia;
		jason->isTouchingSophia = true;
}
void Interactable::Interact(PlayerBullet* bullet, Env_Wall * wall)
{
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();

	if (wallBox.SweptAABB(bulletBox, bullet->dx()) != -INFINITY)
		bullet->SetState(bullet->state | BULLET_STATE_EXPLODE);
}
void Interactable::Interact(PlayerBullet* bullet, Enemy* enemy) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox enemyBox = enemy->GetBoundingBox();
	if (enemyBox.SweptAABB(bulletBox, bullet->dx() + enemy->dx()) != -INFINITY)
	{
		enemy->TakeDamage(bullet->GetDamage());
		bullet->GetManager()->RemoveElement(bullet);
	}
}
#pragma endregion

#undef DAMAGE_OF_SPIKE
