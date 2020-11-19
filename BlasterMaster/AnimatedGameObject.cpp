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
	drawArguments.FlipHorizontal(isFlipHorizontal);
	drawArguments.SetRotation(rotation);

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

void Enemy::TakeDamage(int damage)
{
	HealthPoint -= damage;
	if (HealthPoint < 0)
		manager->RemoveElement(this);
}

void AnimatedScene::Interact(Interactable * other) { other->Interact(this); }
#include "InteractableGroupInclude.h"
#define CURRENT_CLASS AnimatedScene
APPLY_MACRO(INTERACTABLE_DEF_CPP, INTERACTABLE_GROUP)
#undef CURRENT_CLASS

