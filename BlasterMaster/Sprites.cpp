#include "Sprites.h"
#include "Game.h"
#include "Utils.h"
#include "DrawArguments.h"

Sprite::Sprite(int id, BoundingBox boundingBox, LPDIRECT3DTEXTURE9 tex)
{
	this->id = id;
	this->mSpriteRect = boundingBox.toRect();
	this->mTexture = tex;
	this->Init();
}
/*
void Sprite::Draw(Point pos, int alpha)
{
	Game * game = Game::GetInstance();
	game->Draw(pos, texture, boundingBox, alpha);
}
//*/

int Sprite::GetWidth()
{
	return mWidth;
}

void Sprite::SetWidth(int width)
{
	mWidth = width;
}

int Sprite::GetHeight()
{
	return mHeight;
}

void Sprite::SetHeight(int height)
{
	mHeight = height;
}

void SpriteLibrary::Add(int id, BoundingBox boundingBox, LPDIRECT3DTEXTURE9 tex)
{
	Sprite* s = new Sprite(id, boundingBox, tex);
	sprites[id] = s;

	DebugOut(L"[INFO] sprite added: %d, %d, %d, %d, %d \n", id, 
		boundingBox.l, boundingBox.t, 
		boundingBox.r, boundingBox.b);
}

Sprite* SpriteLibrary::Get(int id)
{
	return sprites[id];
}

/*
	Clear all loaded textures
*/
void SpriteLibrary::Clear()
{
	for (auto x : sprites)
	{
		Sprite* s = x.second;
		delete s;
	}

	sprites.clear();
}

void Sprite::Init()
{
	HRESULT result;
	mSpriteHandler = GameGlobal::GetCurrentSpriteHandler();

	//D3DXGetImageInfoFromFileA(filePath, &mImageInfo);
/*
	if (width == NULL)
	{
		if (!isRect(sourceRect))
			mWidth = mImageInfo.Width;
		else
			mWidth = sourceRect.right - sourceRect.left;
	}
	else
		mWidth = width;

	if (height == NULL)
	{
		if (!isRect(sourceRect))
			mHeight = mImageInfo.Height;
		else
			mHeight = sourceRect.bottom - sourceRect.top;
	}
	else
		mHeight = height;
*/
	if (!isRect(mSpriteRect))
	{
		mSpriteRect.left = 0;
		mSpriteRect.right = mWidth;
		mSpriteRect.top = 0;
		mSpriteRect.bottom = mHeight;
	}

	//LPDIRECT3DDEVICE9 device;
	//mSpriteHandler->GetDevice(&device);
}

bool Sprite::isRect(RECT rect)
{
	if (rect.left == rect.right)
		return false;

	if (rect.top == rect.bottom)
		return false;

	return true;
}

void Sprite::Draw(DrawArguments drawArguments)
{
	D3DCOLOR color = drawArguments.GetColor();
	//FlipHorizontal(isFlipHorizontal);
	RECT inSourceRect = mSpriteRect;
	Point pos = drawArguments.GetPosition();
	D3DXVECTOR3 inPosition = D3DXVECTOR3(pos.x, pos.y, 0);
	float inRotation = drawArguments.GetRotation();
	D3DXVECTOR2 inScale = drawArguments.GetScale();
	// position used in translation
	D3DXVECTOR2 inTranslation = D3DXVECTOR2(drawArguments.GetTranslation().x + pos.x, drawArguments.GetTranslation().y + pos.y);
	D3DXVECTOR2 inRotationCenter = drawArguments.GetRotationCenter();
	D3DXVECTOR2 scalingScenter = D3DXVECTOR2((inSourceRect.right - inSourceRect.left) / 2, (inSourceRect.bottom - inSourceRect.top) / 2);
	inTranslation.x = round(inTranslation.x - scalingScenter.x);
	inTranslation.y = round(inTranslation.y - scalingScenter.y);
	

	//if (position.length() != 0)
	//{
	//	inPosition.x = position.x;
	//	inPosition.y = position.y;
	//}

	//if (isRect(sourceRect))
	//	inSourceRect = sourceRect;

	//if (scale != D3DXVECTOR2())
	//	inScale = scale;

	//if (translate.length() != 0) {
	//	inTranslation.x = translate.x;
	//	inTranslation.y = translate.y;
	//}

	//if (rotationCenter != D3DXVECTOR2())
	//	inRotationCenter = rotationCenter;
	//else
	//	mRotationCenter = D3DXVECTOR2(inPosition.x, inPosition.y);// cho phep quay giua hinh

	D3DXMatrixTransformation2D(&mMatrix, &scalingScenter, 0, &inScale, &inRotationCenter,
		inRotation, &inTranslation);

	D3DXMATRIX oldMatrix;
	mSpriteHandler->GetTransform(&oldMatrix);
	mSpriteHandler->SetTransform(&mMatrix);

	D3DXVECTOR3 center = D3DXVECTOR3(mWidth / 2, mHeight / 2, 0);

	mSpriteHandler->Draw(mTexture,
		&inSourceRect,
		&inPosition,
		&inPosition,
		color); 

	mSpriteHandler->SetTransform(&oldMatrix); // set lai matrix cu~ de Sprite chi ap dung transfrom voi class nay
}

void Sprite::SetSourceRect(BoundingBox boundingbox)
{
	mSpriteRect = boundingbox.toRect();
}

LPDIRECT3DTEXTURE9 Sprite::GetTexture()
{
	return mTexture;
}

D3DXIMAGE_INFO Sprite::GetImageInfo()
{
	return mImageInfo;
}

