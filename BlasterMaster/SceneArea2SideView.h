#pragma once
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "GameMap.h"
#include "Environment.h"
#include "HealthBar.h"
#include "Manager.h"
#include <unordered_set>

class Game;

class SceneArea2SideView: public Scene, public Manager<GameObject>
{
protected: 
	//CMario *player;					// A play scene has to have player, right?  // No =)
	
	unordered_set<GameObject*> objects;
	vector<GameObject*> toRemove;

	//LeSon
	vector<Environment*> environments;

	GameMap* mMap;
	GameMap* foreMap;

	HealthBar* healthBar;

	void LoadContent();

	// camera
	Camera *mCamera;
	//LeSon
	Player* target;

	//vector<HealthBar*> healthBar;
private:
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_ENVIRONMENT(string line);
	//void _ParseSection_MAP(string line, vector<tuple<int, int, int, int, int> > &mapNav);

	Point screenSize;

	//LeSon
	void JumpCheckpoint();

public: 
	SceneArea2SideView(int id, LPCWSTR filePath, Game* game, Point screenSize);
	virtual ~SceneArea2SideView();

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Release();

	// Inherited via Manager
	virtual void AddElement(GameObject *) override;
	virtual void RemoveElement(GameObject *) override;

	// just handle change in one scene
	static BoundingBox cameraLimitAreaOfSection[15];
	static Point startPointInSection[15];
};
