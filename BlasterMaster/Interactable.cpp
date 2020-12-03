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
void Interactable::Interact(Enemy* enemy, Env_Wall* wall) {
	BoundingBox enemyBox = enemy->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (enemyBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(enemyBox.r, wallBox.r) - max(enemyBox.l, wallBox.l);
		float overlapAreaY = min(enemyBox.b, wallBox.b) - max(enemyBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (enemyBox.GetCenter().y > wallBox.GetCenter().y) {
				enemy->wallTop = true;
				// Snap top (player pushed down)
				Point pos = enemy->GetPosition();
				pos.y -= enemyBox.t - wallBox.b;
				enemy->SetPosition(pos);
			}
			else
			{
				enemy->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = enemy->GetPosition();
				pos.y += wallBox.t - enemyBox.b;
				enemy->SetPosition(pos);
			}
		}
		else
		{
			if (enemyBox.GetCenter().x < wallBox.GetCenter().x) {
				enemy->wallRight = true;
				// Snap right (player to left)
				Point pos = enemy->GetPosition();
				pos.x -= enemyBox.r - wallBox.l;
				enemy->SetPosition(pos);
			}
			else
			{
				enemy->wallLeft = true;
				// Snap left (player to right)
				Point pos = enemy->GetPosition();
				pos.x += wallBox.r - enemyBox.l;
				enemy->SetPosition(pos);
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

void Interactable::Interact(Player* player, MiniRedBullet* bullet) {

}

void Interactable::Interact(Env_Wall* wall, MiniRedBullet* bullet) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (bulletBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(bulletBox.r, wallBox.r) - max(bulletBox.l, wallBox.l);
		float overlapAreaY = min(bulletBox.b, wallBox.b) - max(bulletBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (bulletBox.GetCenter().y > wallBox.GetCenter().y) {
				bullet->wallTop = true;
				// Snap top (player pushed down)
				Point pos = bullet->GetPosition();
				pos.y -= bulletBox.t - wallBox.b;
				bullet->SetPosition(pos);
			}
			else
			{
				bullet->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = bullet->GetPosition();
				pos.y += wallBox.t - bulletBox.b;
				bullet->SetPosition(pos);
			}
		}
		else
		{
			if (bulletBox.GetCenter().x < wallBox.GetCenter().x) {
				bullet->wallRight = true;
				// Snap right (player to left)
				Point pos = bullet->GetPosition();
				pos.x -= bulletBox.r - wallBox.l;
				bullet->SetPosition(pos);
			}
			else
			{
				bullet->wallLeft = true;
				// Snap left (player to right)
				Point pos = bullet->GetPosition();
				pos.x += wallBox.r - bulletBox.l;
				bullet->SetPosition(pos);
			}
		}
	}
}


void Interactable::Interact(Enemy* enemy, Env_Portal* portal) {
	BoundingBox enemyBox = enemy->GetBoundingBox();
	BoundingBox portalBox = portal->GetBoundingBox();
	
	if (enemyBox.IsOverlap(portalBox)) {
		float overlapAreaX = min(enemyBox.r, portalBox.r) - max(enemyBox.l, portalBox.l);
		float overlapAreaY = min(enemyBox.b, portalBox.b) - max(enemyBox.t, portalBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			
		}
		else
		{
			if (enemyBox.GetCenter().x < portalBox.GetCenter().x) {
				enemy->wallRight = true;
				// Snap right (player to left)
				Point pos = enemy->GetPosition();
				pos.x -= enemyBox.r - portalBox.l;
				enemy->SetPosition(pos);
			}
			else
			{
				enemy->wallLeft = true;
				// Snap left (player to right)
				Point pos = enemy->GetPosition();
				pos.x += portalBox.r - enemyBox.l;
				enemy->SetPosition(pos);
			}
		}
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
			damage = (damage * damage - 1.24) / 1.37;
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
void Interactable::Interact(Bullet* bullet, Env_Wall * wall)
{
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();

	if (wallBox.SweptAABB(bulletBox, bullet->dx()) != -INFINITY)
		bullet->SetState(bullet->state | BULLET_STATE_EXPLODE);
}
void Interactable::Interact(Bullet* bullet, Enemy* enemy) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox enemyBox = enemy->GetBoundingBox();
	if (enemyBox.SweptAABB(bulletBox, bullet->dx() + enemy->dx()) != -INFINITY)
	{
		enemy->TakeDamage(10);
		bullet->GetManager()->RemoveElement(bullet);
		// TODO: Take damage instead of removing enemy
	}
}
#pragma endregion

#undef DAMAGE_OF_SPIKE
