#include "GunBar.h"
#include "Utils.h"
#include "GameGlobal.h"
#include "Game.h"
#include "Camera.h"
#include "JasonOverhead.h"
#include "SceneBoss.h"
#include "SceneArea2Overhead.h"

GunBar::GunBar()
{

}

GunBar::GunBar(TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	Load(texLib, spriteLib);
}

void GunBar::Load(TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	for (size_t i = 0; i < 9; i++)
	{
		// convert str to wstr of tileset path
		std::string texturePathStr = "Resources/sprites/gunspower.png";
		std::wstring texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

		// health bar
		BoundingBox r = BoundingBox(i * 20 + 1, 0, i * 20 + 18, 124);
		texLib->Add(HP_TEXTURE_ID + i, texturePathWstr.c_str(), NULL);
		spriteLib->Add(HP_TEXTURE_ID + i, r, texLib->Get(HP_TEXTURE_ID + i));

		Sprite* sprite = spriteLib->Get(HP_TEXTURE_ID + i);

		mBar.insert(std::pair<int, Sprite*>(i, sprite));
	}
}

void GunBar::Draw()
{
	Point trans = Point(GameGlobal::GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
		GameGlobal::GetHeight() / 2 - Camera::GetInstance()->GetPosition().y);

	Player* currentPlay = NULL;
	Scene* currentScene = Game::GetInstance()->GetCurrentScene();
	if (dynamic_cast<SceneArea2Overhead*>(currentScene) != NULL) {
		currentPlay = dynamic_cast<SceneArea2Overhead*>(currentScene)->GetTarget();
	}
	else if (dynamic_cast<SceneBoss*>(currentScene) != NULL) {
		currentPlay = dynamic_cast<SceneBoss*>(currentScene)->GetTarget();
	}
	int currentGun = dynamic_cast<JasonOverhead*>(currentPlay)->bulletPower;
	int barId;
	if (currentGun >= 80) {
		currentGun = 80;
	}
	if (currentGun < 0) {
		currentGun = 0;
	}
	BoundingBox sourceRECT;
	Sprite* sprite = mBar[currentGun / 10];

	DrawArguments drawArguments;
	Point cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments.SetPosition(cameraPos - Point(120, 55));
	drawArguments.SetTranslation(trans);
	drawArguments.SetScale(D3DXVECTOR2(0.5, 0.5));

	sprite->Draw(drawArguments);
}

void GunBar::Release()
{
}

GunBar::~GunBar()
{
	Release();
}
