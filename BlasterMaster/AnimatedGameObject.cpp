#include "AnimatedGameObject.h"

void AnimatedGameObject::Render()
{
	// set to world coordinate
	Point cameraOffset = Camera::GetInstance()->GetPosition();
	cameraOffset.x -= Camera::GetInstance()->GetWidth() / 2;
	cameraOffset.y -= Camera::GetInstance()->GetHeight() / 2;
	drawArguments.SetPosition(pos - cameraOffset);

	//drawArguments.SetPosition(pos);
	drawArguments.FlipVertical(isFlipVertical);

	currentAnimation->Render(currentTime, previousFrame, drawArguments);
	if (!moving)
		return;
	currentTime++;
	if (currentTime >= currentAnimation->GetLoopDuration())
	{
		currentTime %= currentAnimation->GetLoopDuration();
		previousFrame = 0;
	}
}

void AnimatedGameObject::SetAnimationType(int ANI)
{
	Animation* trg = animationSet->at(ANI);
	if (currentAnimation != trg)
	{
		currentAnimation = trg;
		previousFrame = 0;
		currentTime = 0;
	}
}

void AnimatedScene::Interact(Interactable * other) { other->Interact(this); }
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
#define CURRENT_CLASS AnimatedScene
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS
