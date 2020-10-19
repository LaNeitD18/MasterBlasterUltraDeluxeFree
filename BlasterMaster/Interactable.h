#pragma once

#include "useful_stuff.h"

class Interactable;

// Interactable group 1
class GameObject;
class Enemy;
class Player;
class MapSegment;
class Bullet;
// Interactable group 1

// Interactable group 2
class Environment; // TODO: expand for each environment type
class Player;
class GameObject;
// Interactable group 2

class Interactable
	/** Implement visitor pattern for interacting with self
		Interaction is in the cpp
	*/
{
public:
	Interactable();
	~Interactable();

};

