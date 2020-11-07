#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include "useful_stuff.h"

class DrawArguments
{
protected:
	Point             mPosition; // vi tri cua Sprite, co goc la chinh giua hinh anh Texture

	bool                    mIsFlipVertical, // lat theo chieu doc
							mIsFlipHorizontal; // lat theo chieu ngang
	float                   mRotation; // goc quay cua Sprite tinh theo radian

	D3DXVECTOR2             mScale; // Vector tuy chinh do phong to / thu nho cua texture
	Point             mTranslation; //Doi hinh anh vi tri thuc cua Sprite + mTranslate
	D3DXVECTOR2             mRotationCenter; // diem trung tam trong phep xoay hinh (origin vector)

	D3DCOLOR	mColor;
public:
	DrawArguments();;
	DrawArguments(Point position, bool isFlipHorizontal, D3DXVECTOR2 scale);

	void Init();

	Point GetPosition();
	void SetPosition(D3DXVECTOR3 pos);
	void SetPosition(float x, float y);
	void SetPosition(Point pos);

	void FlipHorizontal(bool flag); // true: lat hinh theo chieu doc, false: binh thuong
	bool IsFlipHorizontal();

	void FlipVertical(bool flag); // true: lat hinh theo chieu ngang, false: binh thuong
	bool IsFlipVertical();

	float GetRotation();
	void SetRotation(float rotation); // by radian

	D3DXVECTOR2 GetScale();
	void SetScale(D3DXVECTOR2 scale);

	Point GetTranslation(); // phep tinh tien tu world position -> view position
	void SetTranslation(Point translation); // phep tinh tien: tu the world position -> view position

	D3DXVECTOR2 GetRotationCenter();
	void SetRotationCenter(D3DXVECTOR2 rotationCenter);

	D3DCOLOR GetColor();
	void SetColor(D3DCOLOR color);
};