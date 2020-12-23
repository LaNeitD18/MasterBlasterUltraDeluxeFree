#include "QuadTree.h"

QuadTree_Node::QuadTree_Node(BoundingBox bbox, int depth) :
	nw{ NULL },
	ne{ NULL },
	sw{ NULL },
	se{ NULL },
	bbox(bbox),
	depth(depth)
{}

QuadTree_Node::~QuadTree_Node()
{
	delete nw;
	delete sw;
	delete ne;
	delete se;
}

void QuadTree_Node::Subdivide()
{
	Point qCentre = bbox.GetCenter();

	nw = new QuadTree_Node(BoundingBox(bbox.l, bbox.t, qCentre.x, qCentre.y), depth + 1);
	ne = new QuadTree_Node(BoundingBox(qCentre.x, bbox.t, bbox.r, qCentre.y), depth + 1);
	sw = new QuadTree_Node(BoundingBox(bbox.l, qCentre.y, qCentre.x, bbox.b), depth + 1);
	se = new QuadTree_Node(BoundingBox(qCentre.x, qCentre.y, bbox.r, bbox.b), depth + 1);

	std::vector<Interactable*> temp;
	for(auto item : objects)
	{
		int insertResult = false;
		if (nw->InsertAndInteract(item, bbox, false))
		{
			insertResult = true;
		} 
		else if (ne->InsertAndInteract(item, bbox, false))
		{
			insertResult = true;
		} 
		else if (sw->InsertAndInteract(item, bbox, false))
		{
			insertResult = true;
		} 
		else if (se->InsertAndInteract(item, bbox, false))
		{
			insertResult = true;
		}
		if (insertResult == false)
			temp.push_back(item);
	}

	std::swap(temp, objects);
}

bool QuadTree_Node::InsertAndInteract(Interactable * object, BoundingBox & const bbox, bool doInteract)
{
	if (!bbox.IsInsideBox(this->bbox))
	{
		return false;
	}

	if (doInteract)
		for (auto item : objects)
		{
			item->Interact(object);
		}

	if (nw == NULL && objects.size() >= depth)
	{
		Subdivide();
	}

	if (nw == NULL)
	{
		objects.push_back(object);
		return true;
	}

	if (nw->InsertAndInteract(object, bbox, doInteract))
	{
		return true;
	}
	if (ne->InsertAndInteract(object, bbox, doInteract))
	{
		return true;
	}
	if (sw->InsertAndInteract(object, bbox, doInteract))
	{
		return true;
	}
	if (se->InsertAndInteract(object, bbox, doInteract))
	{
		return true;
	}

	objects.push_back(object);
	return true;
}

void QuadTree_Node::InteractAll(Interactable * object)
{
	for (auto item : objects)
	{
		item->Interact(object);
	}
	if (nw != NULL)
	{
		nw->InteractAll(object);
		ne->InteractAll(object);
		sw->InteractAll(object);
		se->InteractAll(object);
	}
}

void QuadTree::InsertAndInteract(Interactable * object, BoundingBox bbox, bool IsProximityBased)
{
	if (tree == NULL)
		tree = new QuadTree_Node(BoundingBox(
			QUADTREE_INITIAL_BBOX_LEFT,
			QUADTREE_INITIAL_BBOX_TOP,
			QUADTREE_INITIAL_BBOX_RIGHT,
			QUADTREE_INITIAL_BBOX_BOTTOM
		), 0);

	for (auto item : nonProximityBasedObjects)
	{
		object->Interact(item);
	}
	if (IsProximityBased)
	{
		tree->InsertAndInteract(object, bbox);
	}
	else
	{
		tree->InteractAll(object);
		nonProximityBasedObjects.push_back(object);
	}
}

void QuadTree::InsertToTree(Interactable * object, BoundingBox bbox)
{
	if (tree == NULL)
		tree = new QuadTree_Node(BoundingBox(
			QUADTREE_INITIAL_BBOX_LEFT,
			QUADTREE_INITIAL_BBOX_TOP,
			QUADTREE_INITIAL_BBOX_RIGHT,
			QUADTREE_INITIAL_BBOX_BOTTOM
		), 0);

	tree->InsertAndInteract(object, bbox, false);
}

void QuadTree::Clear()
{
	delete tree;
	tree = NULL;
	nonProximityBasedObjects.clear();
}

QuadTree::QuadTree() :tree{ NULL }
{}

QuadTree::~QuadTree()
{
	delete tree;
}
