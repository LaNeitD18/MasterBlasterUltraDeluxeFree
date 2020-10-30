#include "Animations.h"
#include "Utils.h"

void Animation::Add(int spriteId, SpriteLibrary* spriteLib, DWORD time)
{
	int t = time;
	if (time == 0) t = this->defaultTime;

	Sprite* sprite = spriteLib->Get(spriteId);

	if (sprite == NULL)
	{
		DebugOut(L"[ERROR] Sprite ID %d cannot be found!\n", spriteId);
	}

	loopDuration += t;
	CAnimationFrame* frame = new CAnimationFrame(sprite, loopDuration);
	frames.push_back(frame);
}

void Animation::Render(Point pos, int& time, int& previousFrame, int alpha)
{
	/*
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
	//*/
	if (frames[previousFrame]->GetEndTime() < time)
		previousFrame++;
	frames[previousFrame]->GetSprite()->Draw(pos, RECT(), D3DCOLOR_ARGB(alpha, 255, 255, 255), D3DXVECTOR2(0.25,0.25));
}

void AnimationLibrary::Add(int id, Animation* ani)
{
	animations[id] = ani;
}

Animation* AnimationLibrary::Get(int id)
{
	Animation* ani = animations[id];
	if (ani == NULL)
		DebugOut(L"[ERROR] Failed to find animation id: %d\n", id);
	return ani;
}

void AnimationLibrary::Clear()
{
	for (auto x : animations)
	{
		Animation* ani = x.second;
		delete ani;
	}

	animations.clear();
}

AnimationSets::AnimationSets()
{

}

AnimationSet* AnimationSets::Get(unsigned int id)
{
	AnimationSet* ani_set = animation_sets[id];
	if (ani_set == NULL)
		DebugOut(L"[ERROR] Failed to find animation set id: %d\n",id);
	 
	return ani_set;
}

void AnimationSets::Add(int id, AnimationSet* ani_set)
{
	animation_sets[id] = ani_set;
}
