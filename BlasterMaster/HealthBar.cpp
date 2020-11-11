#include "HealthBar.h"
#include "Utils.h"
#include "GameGlobal.h"
#include "Camera.h"

HealthBar::HealthBar()
{
	
}

HealthBar::HealthBar(TextureLibrary * texLib, SpriteLibrary * spriteLib)
{
	Load(texLib, spriteLib);
}

void HealthBar::Load(TextureLibrary * texLib, SpriteLibrary * spriteLib)
{
	for (size_t i = 0; i < 9; i++)
	{
		// convert str to wstr of tileset path
		std::string texturePathStr = "Resources/sprites/HP.png";
		std::wstring texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

		// health bar
		BoundingBox r = BoundingBox(i * 20+1, 0, i * 20 + 18, 120);
		texLib->Add(HP_TEXTURE_ID+i, texturePathWstr.c_str(), NULL);
		spriteLib->Add(HP_TEXTURE_ID+i, r, texLib->Get(HP_TEXTURE_ID+i));

		Sprite* sprite = spriteLib->Get(HP_TEXTURE_ID+i);


		mBar.insert(std::pair<int, Sprite*>(i, sprite));
	}
}

void HealthBar::Draw()
{
	Point trans = Point(GameGlobal::GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
		GameGlobal::GetHeight() / 2 - Camera::GetInstance()->GetPosition().y);

	int currentHP = GameGlobal::GetHealthPointSideView();
	int barId;
	if (currentHP >= 80) {
		currentHP = 80;
	}
	if (currentHP < 0) {
		return;
	}
	BoundingBox sourceRECT;
	Sprite* sprite = mBar[currentHP / 10];

	DrawArguments drawArguments;
	Point cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments.SetPosition(cameraPos - Point(120,-25));
	drawArguments.SetTranslation(trans);
	drawArguments.SetScale(D3DXVECTOR2(0.5, 0.5));

	sprite->Draw(drawArguments);
}

void HealthBar::Release()
{
}

HealthBar::~HealthBar()
{
	Release();
}
