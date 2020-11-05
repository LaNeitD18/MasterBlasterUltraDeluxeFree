#include "DrawArguments.h"
#include "Utils.h"

DrawArguments::DrawArguments() { Init(); }

DrawArguments::DrawArguments(Point position, bool isFlipHorizontal, D3DXVECTOR2 scale)
{
	this->mPosition = position;
	this->mIsFlipHorizontal = isFlipHorizontal;
	this->mScale = scale;
	Init();
}

void DrawArguments::Init() 
{
	mPosition = Point(0, 0);
	mRotation = 0;
	mRotationCenter = D3DXVECTOR2(mPosition.x, mPosition.y);
	mTranslation = Point(0, 0);
	mScale = D3DXVECTOR2(0, 1);
	mScale.x = mScale.y = 1;
	mIsFlipHorizontal = false;
}

//void DrawArguments::SetPosition(D3DXVECTOR3 pos)
//{
//	mPosition = pos;
//}

void DrawArguments::SetPosition(float x, float y)
{
	//mPosition = D3DXVECTOR3(x, y, 0);
	mPosition = Point(x, y);

}

void DrawArguments::SetPosition(Point pos)
{
	this->SetPosition(pos.x, pos.y);
}

Point DrawArguments::GetPosition()
{
	return mPosition;
}

void DrawArguments::FlipHorizontal(bool flag)
{
	//DebugOut(L"%dflag \n", flag);
	//DebugOut(L"%dhor\n", mIsFlipHorizontal);
	if (mIsFlipHorizontal != flag)
	{
		DebugOut(L"%dflag \n", flag);
		DebugOut(L"%dhor\n", mIsFlipHorizontal);
		mIsFlipHorizontal = flag;
		mScale = D3DXVECTOR2(mScale.x, -mScale.y);
		DebugOut(L"ZZZ\n");
		DebugOut(L"%dhor\n", mIsFlipHorizontal);
	}
}

bool DrawArguments::IsFlipHorizontal()
{
	return mIsFlipHorizontal;
}

void DrawArguments::FlipVertical(bool flag)
{
	if (mIsFlipVertical != flag)
	{
		mIsFlipVertical = flag;
		mScale = D3DXVECTOR2(-mScale.x, mScale.y);
	}
}

bool DrawArguments::IsFlipVertical()
{
	return mIsFlipVertical;
}

float DrawArguments::GetRotation()
{
	return mRotation;
}

void DrawArguments::SetRotation(float rotation)
{
	mRotation = rotation;
}

D3DXVECTOR2 DrawArguments::GetScale()
{
	return mScale;
}

void DrawArguments::SetScale(D3DXVECTOR2 scale)
{
	mScale = scale;
}

Point DrawArguments::GetTranslation()
{
	return mTranslation;
}

void DrawArguments::SetTranslation(Point translation)
{
	mTranslation = translation;
}

D3DXVECTOR2 DrawArguments::GetRotationCenter()
{
	return mRotationCenter;
}

void DrawArguments::SetRotationCenter(D3DXVECTOR2 rotationCenter)
{
	mRotationCenter = rotationCenter;
}