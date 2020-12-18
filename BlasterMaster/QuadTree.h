#pragma once
#include "BoundingBox.h"
#include "Interactable.h"
#include <vector>

#define QUADTREE_INITIAL_BBOX_TOP -8
#define QUADTREE_INITIAL_BBOX_LEFT -8
#define QUADTREE_INITIAL_BBOX_RIGHT 4088
#define QUADTREE_INITIAL_BBOX_BOTTOM 4088

class QuadTree_Node final
{
	QuadTree_Node* nw;
	QuadTree_Node* ne;
	QuadTree_Node* sw;
	QuadTree_Node* se;

	BoundingBox bbox;
	std::vector<Interactable*> objects;

	// The deeper the node, the more element it takes to subdivide.
	int depth;
public:
	QuadTree_Node(BoundingBox bbox, int depth);
	~QuadTree_Node();

	void Subdivide();
	bool InsertAndInteract(Interactable*, BoundingBox& const, bool doInteract = true);
	void InteractAll(Interactable*);
};

class QuadTree final
{
	QuadTree_Node* tree;
	std::vector<Interactable*> nonProximityBasedObjects;
public:
	void InsertAndInteract(Interactable*, BoundingBox, bool IsProximityBased = true);
	void InsertToTree(Interactable*, BoundingBox);
	void Clear();

	QuadTree();
	~QuadTree();
};
