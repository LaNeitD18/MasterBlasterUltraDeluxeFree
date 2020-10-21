#pragma once

#include "useful_stuff.h"
#include "Sprites.h"
#include "Interactable.h"
#include "GameObject.h"
#define MAX_SEGMENT_SIZE 128

// ADDED
// NAK tien
// NAK son
class Environment {
	
};

class EmptyEnvironment : public Environment {};

class MapSegment : public Interactable, public GameObject
{
public:
	MapSegment(TextureLibrary* registerInfo, int sprite_id, const char* sprite_file,
		const char* environmentInfo, RECT& boundingBox);
	virtual ~MapSegment();

	// Note: Lazy init
	void PartialInit(MapSegment* top, MapSegment* left, MapSegment* bottom, MapSegment* right);
	void FullInit();
	//void Render();
	void Release();

	MapSegment* left;
	MapSegment* right;
	MapSegment* top;
	MapSegment* bottom;

private:
	vector<vector<Environment*> > environment;

	// Lazy initialization info
	TextureLibrary* textureLib;
	int sprite_id;
	const char* environmentInfo;
	const char* sprite_file;
	RECT boundingBox;
	Sprite* sprite;

	// Inherited via GameObject
	virtual void GetBoundingBox(BoundingBox& box) override
	{
		box = BoundingBox(boundingBox.left, boundingBox.top,
			boundingBox.right, boundingBox.bottom);
	};
	virtual void Update() override {};
};

