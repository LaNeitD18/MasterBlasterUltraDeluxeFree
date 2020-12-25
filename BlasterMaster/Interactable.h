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
#define DUPLICATE_(...)  DUPLICATE4 (EXPAND(DUPLICATE4 (EXPAND(DUPLICATE4 (EXPAND(__VA_ARGS__))))))

#define APPLY_MACRO_END(...)
#define APPLY_MACRO_GET_END() 0, APPLY_MACRO_END
#define APPLY_MACRO_NEXT0(test, next, ...) next APPLY_MACRO_OUT
#define APPLY_MACRO_NEXT1(test, next) APPLY_MACRO_NEXT0 (test, next, 0)
#define APPLY_MACRO_NEXT(test, next)  APPLY_MACRO_NEXT1 APPLY_MACRO_OUT (EXPAND(APPLY_MACRO_GET_END test), next)

#define APPLY_MACRO0(f, x, peek, ...) f(x) APPLY_MACRO_NEXT (peek, APPLY_MACRO1) (f, peek, EXPAND(__VA_ARGS__))
#define APPLY_MACRO1(f, x, peek, ...) f(x) APPLY_MACRO_NEXT (peek, APPLY_MACRO0) (f, peek, EXPAND(__VA_ARGS__))

/**	Call Macro ARG0 for every other arguments. ARG0(ARG1) ARG0(ARG2) and so on
*/
#define APPLY_MACRO(f, ...) DUPLICATE_ (APPLY_MACRO1 (f, __VA_ARGS__, (), 0))
#pragma endregion

// add new item
#define INTERACTABLE_GROUP Player, Enemy, Env_Wall, Sophia, Env_Spike, Env_Lava, Env_Portal, AnimatedScene, Env_Dungeon, Env_Outdoor, Env_Ladder, ItemPower, ItemHover, Breakable_Tree, JasonOverheadBulletNorm, BossBullet, \
							Worm, Floater, Dome, Jumper, Insect, Ship, MiniRedBullet, SkullBullet, CannonBullet, MineBullet, Teleporter, Eyeball, Boss, BossArm, \
							JasonSideView, JasonOverhead, Bullet, PlayerBullet, EnemyBullet \

// Interactable
#define ___CLASS(x) class x;
APPLY_MACRO(___CLASS, INTERACTABLE_GROUP)

class Bullet;
// Interactable
class Interactable
	/** Implement visitor pattern for interacting with self
		Interaction is in the cpp
	*/
{
public:
	Interactable();
	virtual ~Interactable();

	virtual void Interact(Interactable* other) = 0;

#define INTERACTABLE_DEF_H(groupmember) virtual void Interact(groupmember *) ;
#define INTERACTABLE_DEF_CPP(groupmember) void CURRENT_CLASS :: Interact(groupmember * other) { Interactable::Interact(this, other);}
	/* //COPY THIS TO ALL INTERACTABLE CLASS HEADER FILES
	virtual void Interact(Interactable* other);
	APPLY_MACRO(INTERACTABLE_DEF_H, INTERACTABLE_GROUP);
	//*/
	/* //COPY THIS TO ALL INTERACTABLE CLASS CPP FILES
	#include "InteractableGroupInclude.h"
	#define CURRENT_CLASS <Current class>
	void CURRENT_CLASS :: Interact(Interactable* other) { other->Interact(this); }
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

	INTERACTABLE_MIRROR_DEF(JasonSideView, Env_Wall);

	INTERACTABLE_MIRROR_DEF(Sophia, JasonSideView);
	//add to when new interaction comes out
	INTERACTABLE_MIRROR_DEF(Player, Env_Wall);
	INTERACTABLE_MIRROR_DEF(Player, Env_Spike);
    INTERACTABLE_MIRROR_DEF(Player, Env_Lava);
	INTERACTABLE_MIRROR_DEF(Sophia, Env_Portal);
	INTERACTABLE_MIRROR_DEF(JasonSideView, Env_Portal);
	INTERACTABLE_MIRROR_DEF(JasonOverhead, Env_Portal);
	INTERACTABLE_MIRROR_DEF(Player, Env_Dungeon);
	INTERACTABLE_MIRROR_DEF(Player, Env_Outdoor);
	INTERACTABLE_MIRROR_DEF(JasonSideView, Env_Ladder);
	INTERACTABLE_MIRROR_DEF(Player, Breakable_Tree);
	INTERACTABLE_MIRROR_DEF(JasonOverheadBulletNorm, Breakable_Tree);
	// items
	INTERACTABLE_MIRROR_DEF(Player, ItemPower);
	INTERACTABLE_MIRROR_DEF(Player, ItemHover);
    
	INTERACTABLE_MIRROR_DEF(Player, MiniRedBullet);
	INTERACTABLE_MIRROR_DEF(Env_Wall, MiniRedBullet);
	INTERACTABLE_MIRROR_DEF(Player, SkullBullet);
	INTERACTABLE_MIRROR_DEF(Env_Wall, SkullBullet);
	INTERACTABLE_MIRROR_DEF(Player, CannonBullet);
	INTERACTABLE_MIRROR_DEF(Env_Wall, CannonBullet);
	INTERACTABLE_MIRROR_DEF(Player, MineBullet);
	INTERACTABLE_MIRROR_DEF(Env_Wall, MineBullet);
	// Enemies
	INTERACTABLE_MIRROR_DEF(Enemy, Env_Wall);
	INTERACTABLE_MIRROR_DEF(Enemy, Env_Portal);
	//INTERACTABLE_MIRROR_DEF(Insect, Env_Wall);

	INTERACTABLE_MIRROR_DEF(PlayerBullet, Enemy);

	INTERACTABLE_MIRROR_DEF(Player, Enemy);
	
	// Bullet
	INTERACTABLE_MIRROR_DEF(PlayerBullet, Env_Wall);

	// Boss (temp, for test)
	INTERACTABLE_MIRROR_DEF(Boss, Env_Wall);
	INTERACTABLE_MIRROR_DEF(BossArm, Env_Wall);
	INTERACTABLE_MIRROR_DEF(Boss, Player);
	INTERACTABLE_MIRROR_DEF(BossBullet, Player);
	INTERACTABLE_MIRROR_DEF(BossBullet, PlayerBullet);
#undef INTERACTABLE_MIRROR_DEF
};

