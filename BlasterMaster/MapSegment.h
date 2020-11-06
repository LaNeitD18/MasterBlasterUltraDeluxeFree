#pragma once

#include "useful_stuff.h"
#include "Sprites.h"
#include "Interactable.h"
#include "GameObject.h"
#define MAX_SEGMENT_SIZE 128

// ADDED
// NAK tien
// NAK son
/*
class MapSegment : public Interactable, public GameObject
{
public:
	MapSegment(TextureLibrary* registerInfo, int sprite_id, const char* sprite_file,
		const char* environmentInfo, BoundingBox& boundingBox);
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
	BoundingBox boundingBox;
	Sprite* sprite;

	// Inherited via GameObject
	virtual BoundingBox GetBoundingBox() override
	{
		return BoundingBox(boundingBox.l, boundingBox.r,
			boundingBox.r, boundingBox.b);
	};
	virtual void Update() override {};
};
//*/
