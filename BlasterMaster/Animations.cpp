#include "Animations.h"
#include "Utils.h"

CAnimationSets * CAnimationSets::__instance = NULL;

void CAnimation::Add(int spriteId, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	Sprite* sprite = SpriteLibrary::GetInstance()->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	CAnimationFrame* frame = new CAnimationFrame(sprite, t);
	frames.push_back(frame);
}

// NOTE: sometimes Animation object is NULL ??? HOW ??? 
void CAnimation::Render(Point pos, int alpha)
{
	DWORD now = GetTickCount();
	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();
		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;
			if (currentFrame == frames.size()) currentFrame = 0;
		}
	}

	frames[currentFrame]->GetSprite()->Draw(pos, alpha);
}

CAnimations * CAnimations::__instance = NULL;

CAnimations * CAnimations::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimations();
	return __instance;
}

void CAnimations::Add(int id, CAnimation* ani)
{
	animations[id] = ani;
}

CAnimation* CAnimations::Get(int id)
{
	CAnimation* ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void CAnimations::Clear()
{
	for (auto x : animations)
	{
		CAnimation* ani = x.second;
		delete ani;
	}

	animations.clear();
}

CAnimationSets::CAnimationSets()
{

}

CAnimationSets *CAnimationSets::GetInstance()
{
	if (__instance == NULL) __instance = new CAnimationSets();
	return __instance;
}

CAnimationSet* CAnimationSets::Get(unsigned int id)
{
	CAnimationSet* ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n",id);
	 
	return ani_set;
}

void CAnimationSets::Add(int id, CAnimationSet* ani_set)
{
	animation_sets[id] = ani_set;
}
