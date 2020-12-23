#include "Sound.h"
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

#define DAMAGE_OF_SPIKE 10
#define DAMAGE_OF_ENEMY 5
#define DAMAGE_OF_LAVA 20

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

//void Interactable::Interact(Player* player, Env_Portal* portal) {
//	// implement interact with portal (section)
//	Input& input = *GameGlobal::GetInput();
//	BoundingBox playerBox = player->GetBoundingBox();
//	BoundingBox portalBox = portal->GetBoundingBox();
//	if (/*playerBox.IsOverlap(portalBox) &&*/ portalBox.IsInsideBox(playerBox.GetCenter())) {
//		GateDirection portalDirection = portal->GetPortalDir();
//		if (((input[VK_RIGHT] & KEY_STATE_DOWN) && portalDirection == RIGHT) ||
//			((input[VK_LEFT] & KEY_STATE_DOWN) && portalDirection == LEFT) ||
//			((input[VK_UP] & KEY_STATE_DOWN) && portalDirection == TOP) ||
//			((input[VK_DOWN] & KEY_STATE_DOWN) && portalDirection == BOTTOM)) {
//			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
//			Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
//			if (portalDirection == RIGHT) {
//				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(1);
//			}
//			else if (portalDirection == LEFT) {
//				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(0);
//			}
//			else if (portalDirection == TOP) {
//				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(2);
//			}
//			else {
//				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(3);
//			}
//			int sectionToEnter = portal->GetSectionToEnter();
//			if (sectionToEnter == -1) {
//				BoundingBox currentCameraLimit = Camera::GetInstance()->GetBound();
//				if (portalDirection == RIGHT) {
//					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l + 256, currentCameraLimit.t, currentCameraLimit.r + 256, currentCameraLimit.b);
//				}
//				else if (portalDirection == LEFT) {
//					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l - 256, currentCameraLimit.t, currentCameraLimit.r - 256, currentCameraLimit.b);
//				}
//				else if (portalDirection == TOP) {
//					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l, currentCameraLimit.t - 256, currentCameraLimit.r, currentCameraLimit.b - 256);
//				}
//				else {
//					Camera::GetInstance()->SetCameraLimitarea(currentCameraLimit.l, currentCameraLimit.t + 256, currentCameraLimit.r, currentCameraLimit.b + 256);
//				}
//			}
//			else {
//				BoundingBox limitArea = NULL;
//				if (dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene())) {
//					limitArea = SceneArea2SideView::cameraLimitAreaOfSection[sectionToEnter];
//					// section BF transition
//					if (portal->GetBoundingBox().l == 1472 && portal->GetBoundingBox().t == 2960) {
//						Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(50);
//					}
//					if (portal->GetBoundingBox().l == 1568 && portal->GetBoundingBox().t == 912) {
//						Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(55);
//					}
//				}
//				else if (dynamic_cast<SceneArea2Overhead*>(Game::GetInstance()->GetCurrentScene())) {
//					limitArea = SceneArea2Overhead::cameraLimitAreaOfSection[sectionToEnter];
//				}
//				Camera::GetInstance()->SetCameraLimitarea(limitArea);
//			}
//			
//			DebugOut(L"%d\n", portalDirection);
//		}
//	}
//}

void Interactable::Interact(Sophia* player, Env_Portal* portal) {
	// implement interact with portal (section)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox portalBox = portal->GetBoundingBox();
	// fix bug case when jason be current player while sophia interaction with portal
	SceneArea2SideView* scene = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
	Sophia* sophia = dynamic_cast<Sophia*>(scene->GetTarget());
	bool isSophiaPlaying = sophia != NULL;
	if (isSophiaPlaying /*&& playerBox.IsOverlap(portalBox)*/ && portalBox.IsInsideBox(playerBox.GetCenter())) {
		GateDirection portalDirection = portal->GetPortalDir();
		if ((/*(input[VK_RIGHT] & KEY_STATE_DOWN)*/ player->GetSpeed().x>0 && portalDirection == RIGHT) ||
			(/*(input[VK_LEFT] & KEY_STATE_DOWN)*/ player->GetSpeed().x<0 && portalDirection == LEFT)) {
			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
			Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
			if (portalDirection == RIGHT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(1);
			}
			else if (portalDirection == LEFT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(0);
			}
			int sectionToEnter = portal->GetSectionToEnter();
			
			BoundingBox limitArea = NULL;
			limitArea = SceneArea2SideView::cameraLimitAreaOfSection[sectionToEnter];
			// section BF transition
			if (portal->GetBoundingBox().l == 1472 && portal->GetBoundingBox().t == 2960) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(50);
			}
			if (portal->GetBoundingBox().l == 1568 && portal->GetBoundingBox().t == 912) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(55);
			}
			Camera::GetInstance()->SetCameraLimitarea(limitArea);

			DebugOut(L"%d\n", portalDirection);
		}
		// implement interact with portal to set return point
		else if((player->GetSpeed().x <= 0 && portalDirection == RIGHT) ||
			(player->GetSpeed().x >= 0 && portalDirection == LEFT)) {
			if (portal->GetPortalDir() == LEFT) {
				GameGlobal::SetReturnPoint(Point(portal->GetBoundingBox().r, portal->GetBoundingBox().t - 4));
			}
			else if (portal->GetPortalDir() == RIGHT) {
				GameGlobal::SetReturnPoint(Point(portal->GetBoundingBox().l, portal->GetBoundingBox().t - 4));
			}
			GameGlobal::SetReturnBoundingBox(Camera::GetInstance()->GetCameraLimitarea());
		}
	}
}

void Interactable::Interact(JasonSideView* player, Env_Portal* portal) {
	// implement interact with portal (section)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox portalBox = portal->GetBoundingBox();
	if (/*playerBox.IsOverlap(portalBox) &&*/ portalBox.IsInsideBox(playerBox.GetCenter())) {
		GateDirection portalDirection = portal->GetPortalDir();
		if ((/*(input[VK_RIGHT] & KEY_STATE_DOWN)*/ player->GetSpeed().x > 0 && portalDirection == RIGHT) ||
			(/*(input[VK_LEFT] & KEY_STATE_DOWN)*/ player->GetSpeed().x < 0 && portalDirection == LEFT)) {
			//Point startPoint = SceneArea2SideView::startPointInSection[portal->GetSectionToEnter()];
			Game::GetInstance()->GetCurrentScene()->SetFreeCamera(true);
			if (portalDirection == RIGHT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(1);
			}
			else if (portalDirection == LEFT) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(0);
			}
			int sectionToEnter = portal->GetSectionToEnter();

			BoundingBox limitArea = NULL;
			limitArea = SceneArea2SideView::cameraLimitAreaOfSection[sectionToEnter];
			// section BF transition
			if (portal->GetBoundingBox().l == 1472 && portal->GetBoundingBox().t == 2960) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(50);
			}
			if (portal->GetBoundingBox().l == 1568 && portal->GetBoundingBox().t == 912) {
				Game::GetInstance()->GetCurrentScene()->SetDirectionEnter(55);
			}
			Camera::GetInstance()->SetCameraLimitarea(limitArea);

			DebugOut(L"%d\n", portalDirection);
		}
		// implement interact with portal to set return point
		else if ((player->GetSpeed().x <= 0 && portalDirection == RIGHT) ||
			(player->GetSpeed().x >= 0 && portalDirection == LEFT)) {
			/*GameGlobal::SetReturnPoint(portal->GetBoundingBox().GetCenter());
			GameGlobal::SetReturnBoundingBox(Camera::GetInstance()->GetCameraLimitarea());*/
		}
	}
}

void Interactable::Interact(JasonOverhead* player, Env_Portal* portal) {
	// implement interact with portal (section)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox portalBox = portal->GetBoundingBox();
	if (/*playerBox.IsOverlap(portalBox) &&*/ portalBox.IsInsideBox(playerBox.GetCenter())) {
		GateDirection portalDirection = portal->GetPortalDir();
		if (((input[VK_RIGHT] & KEY_STATE_DOWN) && portalDirection == RIGHT) ||
			((input[VK_LEFT] & KEY_STATE_DOWN) && portalDirection == LEFT) ||
			((input[VK_UP] & KEY_STATE_DOWN) && portalDirection == TOP) ||
			((input[VK_DOWN] & KEY_STATE_DOWN) && portalDirection == BOTTOM)) {
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
				limitArea = SceneArea2Overhead::cameraLimitAreaOfSection[sectionToEnter];
				Camera::GetInstance()->SetCameraLimitarea(limitArea);
			}

			DebugOut(L"%d\n", portalDirection);
		}
		// implement interact with portal to set return point
		else if ((player->GetSpeed().x <= 0 && portalDirection == RIGHT) ||
			(player->GetSpeed().x >=0 && portalDirection == LEFT) ||
			(player->GetSpeed().y >= 0 && portalDirection == TOP) ||
			(player->GetSpeed().y <= 0 && portalDirection == BOTTOM)) {
			GameGlobal::SetReturnPoint(portal->GetBoundingBox().GetCenter());
			GameGlobal::SetReturnBoundingBox(Camera::GetInstance()->GetCameraLimitarea());
		}
	}
}

void Interactable::Interact(Player* player, Env_Dungeon* dungeon) {
	// implement interact with dungeon (enter Scene overhead)
	Input& input = *GameGlobal::GetInput();
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox dungeonBox = dungeon->GetBoundingBox();
	JasonSideView* jasonPlay = dynamic_cast<JasonSideView*>(player);
	bool isJasonPlay = jasonPlay != NULL;
	if (playerBox.IsOverlap(dungeonBox) && isJasonPlay) {
		if (input[VK_DOWN] & KEY_STATE_DOWN) {
			// set healthpoint sophia to global
			SceneArea2SideView* scene_sideview = dynamic_cast<SceneArea2SideView*>(Game::GetInstance()->GetCurrentScene());
			for (auto x : scene_sideview->GetObjects()) {
				Sophia* sophia = dynamic_cast<Sophia*>(x);
				if (sophia != NULL) {
					GameGlobal::SetCurrentHealthPointSophia(sophia->GetHP());
					break;
				}
			}
			// jason setup
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
					// set hp based on jason sideview
					current_player->SetHP(player->GetHP());
					break;
				}
			}
			scene->GetTarget()->SetPosition(startPoint);
			scene->GetCamera()->SetCameraLimitarea(limitArea);
			GameGlobal::SetReturnBoundingBox(limitArea);
			scene->liveShow = 0;
			//Camera::GetInstance()->SetCameraLimitarea(limitArea);
			Sophia* sophia = dynamic_cast<Sophia*>(jasonPlay->sophia); // sophia null
			GameGlobal::SetLastPositionSophia(sophia->GetPosition());
		}
	}
}

void Interactable::Interact(Player* player, Env_Outdoor* outdoor) {
	// implement interact with outdoor (return scene sideview)
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
					// set hp based on sophia current hp
					current_player->SetHP(GameGlobal::GetCurrentHealthPointSophia());
					break;
				}
			}
			JasonSideView* jason = new JasonSideView(startPoint.x, startPoint.y);
			current_player->jason = jason;
			jason->sophia = current_player;
			jason->SetManager(current_player->GetManager());
			jason->v.x = 0;
			jason->v.y = -JASON_ENTER_VEHICLE_DISAPPEAR_SPEED;
			current_player->GetManager()->AddElement(jason);
			current_player->SetState(SOPHIA_STATE_LEFT_VEHICLE);
			scene->SetTarget(jason);
			// set hp based on jason SceneOverhead
			jason->SetHP(player->GetHP());
			//scene->GetTarget()->SetPosition(startPoint);
			scene->GetCamera()->SetCameraLimitarea(limitArea);
			GameGlobal::SetReturnBoundingBox(limitArea);
			//Camera::GetInstance()->SetCameraLimitarea(limitArea);
		}
	}
}

#define POWER_GAIN 10

void Interactable::Interact(Player* player, ItemPower* item) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox itemBox = item->GetBoundingBox();
	if (playerBox.IsOverlap(itemBox)) {
		player->SetHP(player->GetHP() + POWER_GAIN);
		item->GetManager()->RemoveElement(item);
	}
}

void Interactable::Interact(Player* player, ItemHover* item) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox itemBox = item->GetBoundingBox();
	if (playerBox.IsOverlap(itemBox)) {
		//TODO: setup hover for sophia
		//displayMessage("i want to get this");
		item->GetManager()->RemoveElement(item);
	}
}

void Interactable::Interact(Player* player, Breakable_Tree* tree) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox treeBox = tree->GetBoundingBoxJason();
	//if (playerBox.IsOverlap(treeBox)) {
		bool top, left, right, bottom;
		Point move = player->dx();
		top = left = right = bottom = false;
		double offsetTime = treeBox.SweptAABB(playerBox, move, top, left, bottom, right);

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
	//}
}

void Interactable::Interact(JasonOverheadBulletNorm* bullet, Breakable_Tree* tree) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox treeBox = tree->GetBoundingBox();
	
	if (treeBox.SweptAABB(bulletBox, bullet->dx()) != -INFINITY) {
		bullet->SetState(bullet->state | BULLET_STATE_EXPLODE);
		tree->SetIsOut(true);
	}
}


#pragma region Tien
void Interactable::Interact(Enemy* enemy, Env_Wall* wall) {
	BoundingBox enemyBox = enemy->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	/* TODO: convert to SweptAABB. This Currently breaks Dome & Floater
	bool top, left, right, bottom;
	Point move = enemy->dx();
	top = left = right = bottom = false;
	double offsetTime = wallBox.SweptAABB(enemyBox, move, top, left, bottom, right);

	enemy->wallTop |= top;
	enemy->wallRight |= right;
	enemy->wallLeft |= left;
	enemy->wallBot |= bottom;

	if (offsetTime >= 0.0 && offsetTime <= 1.0)
		move = move - move * offsetTime;
	else
		return;

	if (top || bottom) {
		Point v = enemy->GetSpeed();
		v.y -= move.y;
		enemy->SetSpeed(v);
	}
	if (left || right) {
		Point v = enemy->GetSpeed();
		v.x -= move.x;
		enemy->SetSpeed(v);
	}
	/*/
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
	//*/
}

void Interactable::Interact(Player* player, Enemy* enemy) {
	// implement interact between player and enemy (take damage)
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox enemyBox = enemy->GetBoundingBox();
	if (playerBox.IsOverlap(enemyBox)) {
		JasonSideView* jasonPlay = dynamic_cast<JasonSideView*>(player);
		bool isJasonPlay = jasonPlay != NULL;
		int enemyDamage = DAMAGE_OF_ENEMY;
		if (isJasonPlay) {
			enemyDamage *= 2;
		}
		else if (dynamic_cast<Boss*>(enemy) != NULL || dynamic_cast<BossArm*>(enemy) != NULL || dynamic_cast<BossClaw*>(enemy) != NULL) {
			enemyDamage *= 2;
		}
		player->TakeDamage(enemyDamage);
		enemy->isCollided = true;
	}
}

void Interactable::Interact(Player* player, MiniRedBullet* bullet) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox playerBox = player->GetBoundingBox();
	if (playerBox.SweptAABB(bulletBox, bullet->dx() + player->dx()) != -INFINITY)
	{
		player->TakeDamage(10);
	}
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

void Interactable::Interact(Player* player, SkullBullet* bullet) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox playerBox = player->GetBoundingBox();
	if (playerBox.SweptAABB(bulletBox, bullet->dx() + player->dx()) != -INFINITY)
	{
		// SKULL_BULLET_STATE_NORMAL = 100
		if (bullet->GetState() == 100) {
			player->TakeDamage(10);
		}
	}
}

void Interactable::Interact(Env_Wall* wall, SkullBullet* bullet) {
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

void Interactable::Interact(Player* player, CannonBullet* bullet) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox playerBox = player->GetBoundingBox();
	if (playerBox.SweptAABB(bulletBox, bullet->dx() + player->dx()) != -INFINITY)
	{
		// CANNON_BULLET_STATE_NORMAL = 100
		if (bullet->GetState() == 100) {
			player->TakeDamage(10);
		}
	}
}

void Interactable::Interact(Env_Wall* wall, CannonBullet* bullet) {
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

void Interactable::Interact(Player* player, MineBullet* bullet) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox playerBox = player->GetBoundingBox();
	if (playerBox.SweptAABB(bulletBox, bullet->dx() + player->dx()) != -INFINITY)
	{
		// MINE_BULLET_STATE_NORMAL = 100
		if (bullet->GetState() == 100) {
			player->TakeDamage(10);
		}
	}
}

void Interactable::Interact(Env_Wall* wall, MineBullet* bullet) {
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
				//bullet->SetPosition(pos);
			}
			else
			{
				bullet->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = bullet->GetPosition();
				pos.y += wallBox.t - bulletBox.b;
				//bullet->SetPosition(pos);
			}
		}
		else
		{
			if (bulletBox.GetCenter().x < wallBox.GetCenter().x) {
				bullet->wallRight = true;
				// Snap right (player to left)
				Point pos = bullet->GetPosition();
				pos.x -= bulletBox.r - wallBox.l;
				//bullet->SetPosition(pos);
			}
			else
			{
				bullet->wallLeft = true;
				// Snap left (player to right)
				Point pos = bullet->GetPosition();
				pos.x += wallBox.r - bulletBox.l;
				//bullet->SetPosition(pos);
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

	HomingBullet* homing = dynamic_cast<HomingBullet*>(bullet);
	if (homing != NULL)
		return;
	if (wallBox.SweptAABB(bulletBox, bullet->dx()) != -INFINITY)
		bullet->SetState(bullet->state | BULLET_STATE_EXPLODE);
}
void Interactable::Interact(PlayerBullet* bullet, Enemy* enemy) {
	BoundingBox bulletBox = bullet->GetBoundingBox();
	BoundingBox enemyBox = enemy->GetBoundingBox();
	if (enemyBox.SweptAABB(bulletBox, bullet->dx() + enemy->dx()) != -INFINITY)
	{
		enemy->TakeDamage(bullet->GetDamage());
		//DebugOut(L"Damage %d,",bullet->GetDamage());
		bool isGrenadeBullet = dynamic_cast<JasonOverheadBulletGrenadeFragment*>(bullet) != NULL || dynamic_cast<JasonOverheadBulletGrenade*>(bullet) != NULL;
		if (!isGrenadeBullet) {
			bullet->SetState(bullet->state | BULLET_STATE_EXPLODE);
		}
	}
	HomingBullet* homing = dynamic_cast<HomingBullet*>(bullet);
	if (homing != NULL) {
		if (homing->proposedTarget == NULL || (homing->proposedTarget->pos - homing->pos).length() > (homing->pos - enemy->pos).length())
			homing->proposedTarget = enemy;
		if (homing->target == (GameObject*)enemy)
			homing->isTargetAvailable = true;
	}
}
void Interactable::Interact(JasonSideView * player, Env_Ladder * ladder) {
	BoundingBox playerBox = player->GetBoundingBox();
	BoundingBox ladderBox = ladder->GetBoundingBox();

	// Bug is hidden =)																																																				Author: Long
	BoundingBox ladderBoxBigger = ladder->GetBoundingBox();
	ladderBoxBigger.t -= JASON_BBOX_OFFSET_BOTTOM - JASON_BBOX_OFFSET_TOP_NORMAL;
	if (playerBox.IsInsideBox(ladderBoxBigger))
		player->targetLadder = ladder;

	BoundingBox ladderBoxSmaller = ladder->GetBoundingBox();
	ladderBoxSmaller.t += JASON_CLIMBING_SPEED;
	if (playerBox.IsOverlap(ladderBoxSmaller))
		return;

	bool top, left, right, bottom;
	Point move = player->dx();
	top = left = right = bottom = false;

	double offsetTime = ladderBox.SweptAABB(playerBox, move, top, left, bottom, right);

	player->wallBot |= bottom;

	if (offsetTime >= 0.0 && offsetTime <= 1.0) {
		move = move - move * offsetTime;

		if (bottom && move.y > 0) {
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
	}
}
void Interactable::Interact(Boss* boss , Env_Wall* wall ) {
	BoundingBox playerBox = boss->GetBoundingBox();
	BoundingBox wallBox = wall->GetBoundingBox();
	if (playerBox.IsOverlap(wallBox)) {
		float overlapAreaX = min(playerBox.r, wallBox.r) - max(playerBox.l, wallBox.l);
		float overlapAreaY = min(playerBox.b, wallBox.b) - max(playerBox.t, wallBox.t);
		if (overlapAreaX > overlapAreaY)
		{
			if (playerBox.GetCenter().y > wallBox.GetCenter().y) {
				boss->wallTop = true;
				// Snap top (player pushed down)
				Point pos = boss->GetPosition();
				pos.y -= playerBox.t - wallBox.b;
				boss->SetPosition(pos);
			}
			else
			{
				boss->wallBot = true;
				// Snap bottom (player pushed up)
				Point pos = boss->GetPosition();
				pos.y += wallBox.t - playerBox.b;
				boss->SetPosition(pos);
			}
		}
		else
		{
			if (playerBox.GetCenter().x < wallBox.GetCenter().x) {
				boss->wallRight = true;
				// Snap right (player to left)
				Point pos = boss->GetPosition();
				pos.x -= playerBox.r - wallBox.l;
				boss->SetPosition(pos);
			}
			else
			{
				boss->wallLeft = true;
				// Snap left (player to right)
				Point pos = boss->GetPosition();
				pos.x += wallBox.r - playerBox.l;
				boss->SetPosition(pos);
			}
		}
	}
}
void Interactable::Interact(BossArm * , Env_Wall * ) {}
#pragma endregion

#undef DAMAGE_OF_SPIKE
