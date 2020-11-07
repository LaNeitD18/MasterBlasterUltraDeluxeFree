#pragma once

#include "useful_stuff.h"

class Interactable;

// Interactions explicitely
// Player - Enemy (n^2)
// Enemy - Wall (quad tree)
// Player bullet - Enemy (quad tree)
// Player - Environment (quad tree)

#pragma region APPLY MACRO

#define EXPAND(x) x

#define APPLY_MACRO_OUT

#define A(x) x B APPLY_MACRO_OUT (x)
#define B(x) x A APPLY_MACRO_OUT (x)

#define DUPLICATE0(...) EXPAND(__VA_ARGS__)
#define DUPLICATE1(...) DUPLICATE0 (EXPAND(DUPLICATE0 (EXPAND(DUPLICATE0 (EXPAND(__VA_ARGS__))))))
#define DUPLICATE2(...) DUPLICATE1 (EXPAND(DUPLICATE1 (EXPAND(DUPLICATE1 (EXPAND(__VA_ARGS__))))))
#define DUPLICATE3(...) DUPLICATE2 (EXPAND(DUPLICATE2 (EXPAND(DUPLICATE2 (EXPAND(__VA_ARGS__))))))
#define DUPLICATE4(...) DUPLICATE3 (EXPAND(DUPLICATE3 (EXPAND(DUPLICATE3 (EXPAND(__VA_ARGS__))))))
#define DUPLICATE(...)  DUPLICATE4 (EXPAND(DUPLICATE4 (EXPAND(DUPLICATE4 (EXPAND(__VA_ARGS__))))))

#define APPLY_MACRO_END(...)
#define APPLY_MACRO_GET_END() 0, APPLY_MACRO_END
#define APPLY_MACRO_NEXT0(test, next, ...) next APPLY_MACRO_OUT
#define APPLY_MACRO_NEXT1(test, next) APPLY_MACRO_NEXT0 (test, next, 0)
#define APPLY_MACRO_NEXT(test, next)  APPLY_MACRO_NEXT1 APPLY_MACRO_OUT (EXPAND(APPLY_MACRO_GET_END test), next)

#define APPLY_MACRO0(f, x, peek, ...) f(x) APPLY_MACRO_NEXT (peek, APPLY_MACRO1) (f, peek, EXPAND(__VA_ARGS__))
#define APPLY_MACRO1(f, x, peek, ...) f(x) APPLY_MACRO_NEXT (peek, APPLY_MACRO0) (f, peek, EXPAND(__VA_ARGS__))

/**	Call Macro ARG0 for every other arguments. ARG0(ARG1) ARG0(ARG2) and so on
*/
#define APPLY_MACRO(f, ...) DUPLICATE (APPLY_MACRO1 (f, __VA_ARGS__, (), 0))
#pragma endregion

// add new item
#define INTERACTABLE_GROUP Player, Enemy, Env_Wall, Sophia, Env_Spike, Env_Lava, \
							Worm //Bullet

// Interactable
#define ___CLASS(x) class x;
APPLY_MACRO(___CLASS, INTERACTABLE_GROUP)
// Interactable
class Interactable
	/** Implement visitor pattern for interacting with self
		Interaction is in the cpp
	*/
{
public:
	Interactable();
	~Interactable();

	virtual void Interact(Interactable* other) = 0;

#define INTERACTABLE_DEF_H(groupmember) virtual void Interact(groupmember *) ;
#define INTERACTABLE_DEF_CPP(groupmember) void CURRENT_CLASS :: Interact(groupmember * other) { Interactable::Interact(this, other);}
	/* //COPY THIS TO ALL INTERACTABLE CLASS HEADER FILES
	virtual void Interact(Interactable* other) { other->Interact(this); }
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	//*/
	/* //COPY THIS TO ALL INTERACTABLE CLASS CPP FILES
	#include "Sophia.h"
	#include "Environment.h"
	#include "Dome.h"
	#include "Eye.h"
	#include "Floater.h"
	#include "Insect.h"
	#include "Jason.h"
	#include "Jumper.h"
	#include "Laser.h"
	#include "Mine.h"
	#include "Orb.h"
	#include "Worm.h"
	#include "Teleporter.h"
	#include "Walker.h"
	#define CURRENT_CLASS <Current class>
	APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
	#undef CURRENT_CLASS
	//*/
#define INTERACTABLE_DEF_VIRTUAL(groupmember) virtual void Interact(groupmember *) = 0;
	APPLY_MACRO(INTERACTABLE_DEF_VIRTUAL, INTERACTABLE_GROUP);
#undef INTERACTABLE_DEF_HERE
	//*/
	/** Undefined Interactions goes here*/
	void Interact(Interactable*, Interactable*){}

#define INTERACTABLE_MIRROR_DEF(mem1, mem2)											\
	void Interact(mem1 *, mem2 *);													\
	inline void Interact(mem2 * one, mem1 * other)									\
		{ Interact(other, one); }
	//INTERACTABLE_MIRROR_DEF(Player, Enemy);
	//INTERACTABLE_MIRROR_DEF(Bullet, Enemy);

	//add to when new interaction comes out
	INTERACTABLE_MIRROR_DEF(Player, Env_Wall);
	INTERACTABLE_MIRROR_DEF(Player, Env_Spike);
    INTERACTABLE_MIRROR_DEF(Player, Env_Lava);
    
	// Enemies
	INTERACTABLE_MIRROR_DEF(Worm, Env_Wall);
	
	
#undef INTERACTABLE_MIRROR_DEF
};

