#include "SceneBullet.h"
#include "Utils.h"
#include "GameGlobal.h"
#include "Camera.h"
#include "Sound.h"

SceneBullet::SceneBullet()
{

}

SceneBullet::SceneBullet(TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	Load(texLib, spriteLib);
}

void SceneBullet::SelectType()
{
	Input& input = *GameGlobal::GetInput();
	if ((input[VK_LEFT] & KEY_STATE_DOWN)) {
		GameGlobal::SetSpecialBulletType(1);
		Sound::getInstance()->play("scene_change", false, 1);
	}
	else if ((input[VK_DOWN] & KEY_STATE_DOWN)) {
		GameGlobal::SetSpecialBulletType(2);
		Sound::getInstance()->play("scene_change", false, 1);
	}
	else if ((input[VK_RIGHT] & KEY_STATE_DOWN)) {
		GameGlobal::SetSpecialBulletType(3);
		Sound::getInstance()->play("scene_change", false, 1);
	}
}

void SceneBullet::Load(TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	// convert str to wstr of tileset path
	std::string texturePathStr = "Resources/sprites/bullet_scene.png";
	std::wstring texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

	// scene bullet
	BoundingBox r = BoundingBox(0, 0, 256, 240);
	texLib->Add(BULLET_SCENE_TEXTURE_ID, texturePathWstr.c_str(), NULL);
	spriteLib->Add(BULLET_SCENE_TEXTURE_ID, r, texLib->Get(BULLET_SCENE_TEXTURE_ID));

	base = spriteLib->Get(BULLET_SCENE_TEXTURE_ID);

	// convert str to wstr of tileset path
	texturePathStr = "Resources/sprites/roundselection.png";
	texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

	// button select
	r = BoundingBox(0, 0, 30, 30);
	texLib->Add(BUTTON_TEXTURE_ID, texturePathWstr.c_str(), NULL);
	spriteLib->Add(BUTTON_TEXTURE_ID, r, texLib->Get(BUTTON_TEXTURE_ID));

	selection = spriteLib->Get(BUTTON_TEXTURE_ID);

	for (size_t i = 0; i < 10; i++)
	{
		// convert str to wstr of tileset path
		std::string texturePathStr = "Resources/sprites/nums.png";
		std::wstring texturePathWstr = std::wstring(texturePathStr.begin(), texturePathStr.end());

		// health bar
		BoundingBox r = BoundingBox(i * 9, 0, i * 9 + 9, 10);
		texLib->Add(NUMS_TEXTURE_ID + i, texturePathWstr.c_str(), NULL);
		spriteLib->Add(NUMS_TEXTURE_ID + i, r, texLib->Get(NUMS_TEXTURE_ID + i));

		Sprite* sprite = spriteLib->Get(NUMS_TEXTURE_ID + i);

		nums.insert(std::pair<int, Sprite*>(i, sprite));
	}
}

void SceneBullet::Draw()
{
	Point trans = Point(GameGlobal::GetWidth() / 2 - Camera::GetInstance()->GetPosition().x,
		GameGlobal::GetHeight() / 2 - Camera::GetInstance()->GetPosition().y);

	// draw base
	DrawArguments drawArguments_scene;
	Point cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_scene.SetPosition(cameraPos - Point(8, 18));
	drawArguments_scene.SetTranslation(trans);
	drawArguments_scene.SetScale(D3DXVECTOR2(1, 1));
	base->Draw(drawArguments_scene);

	//int bullet1 = GameGlobal::GetCurrenBullet1();
	int bullet1 = GameGlobal::GetNumberSpecialBullet1();
	int bullet2 = GameGlobal::GetNumberSpecialBullet2();
	int bullet3 = GameGlobal::GetNumberSpecialBullet3();
	if (bullet1 >= 99) {
		bullet1 = 99;
	}
	if (bullet1 < 0) {
		return;
	}
	if (bullet2 >= 99) {
		bullet1 = 99;
	}
	if (bullet2 < 0) {
		return;
	}
	if (bullet3 >= 99) {
		bullet1 = 99;
	}
	if (bullet3 < 0) {
		return;
	}

	int firstunit_bullet1 = bullet1 / 10;
	int secondunit_bullet1 = bullet1 % 10;
	int firstunit_bullet2 = bullet2 / 10;
	int secondunit_bullet2 = bullet2 % 10;
	int firstunit_bullet3 = bullet3 / 10;
	int secondunit_bullet3 = bullet3 % 10;
	
	BoundingBox sourceRECT;
	Sprite* sprite1 = nums[firstunit_bullet1];

	DrawArguments drawArguments_firstunit1;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_firstunit1.SetPosition(cameraPos - Point(44, -58));
	drawArguments_firstunit1.SetTranslation(trans);
	drawArguments_firstunit1.SetScale(D3DXVECTOR2(1, 1));

	Sprite* sprite2 = nums[secondunit_bullet1];

	DrawArguments drawArguments_secondunit1;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_secondunit1.SetPosition(cameraPos - Point(36, -58));
	drawArguments_secondunit1.SetTranslation(trans);
	drawArguments_secondunit1.SetScale(D3DXVECTOR2(1, 1));

	Sprite* sprite3 = nums[firstunit_bullet2];

	DrawArguments drawArguments_firstunit2;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_firstunit2.SetPosition(cameraPos - Point(12, -58));
	drawArguments_firstunit2.SetTranslation(trans);
	drawArguments_firstunit2.SetScale(D3DXVECTOR2(1, 1));

	Sprite* sprite4 = nums[secondunit_bullet2];

	DrawArguments drawArguments_secondunit2;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_secondunit2.SetPosition(cameraPos - Point(4, -58));
	drawArguments_secondunit2.SetTranslation(trans);
	drawArguments_secondunit2.SetScale(D3DXVECTOR2(1, 1));

	Sprite* sprite5 = nums[firstunit_bullet3];

	DrawArguments drawArguments_firstunit3;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_firstunit3.SetPosition(cameraPos - Point(-20, -58));
	drawArguments_firstunit3.SetTranslation(trans);
	drawArguments_firstunit3.SetScale(D3DXVECTOR2(1, 1));

	Sprite* sprite6 = nums[secondunit_bullet3];

	DrawArguments drawArguments_secondunit3;
	cameraPos = Camera::GetInstance()->GetPosition();
	drawArguments_secondunit3.SetPosition(cameraPos - Point(-28, -58));
	drawArguments_secondunit3.SetTranslation(trans);
	drawArguments_secondunit3.SetScale(D3DXVECTOR2(1, 1));

	sprite1->Draw(drawArguments_firstunit1);
	sprite2->Draw(drawArguments_secondunit1);
	sprite3->Draw(drawArguments_firstunit2);
	sprite4->Draw(drawArguments_secondunit2);
	sprite5->Draw(drawArguments_firstunit3);
	sprite6->Draw(drawArguments_secondunit3);

	SelectType();
	DrawArguments buttonSelect;
	cameraPos = Camera::GetInstance()->GetPosition();
	if (GameGlobal::GetSpecialBulletType() == 1) {
		buttonSelect.SetPosition(cameraPos - Point(56, -45));
	}
	else if (GameGlobal::GetSpecialBulletType() == 2) {
		buttonSelect.SetPosition(cameraPos - Point(24, -45));
	}
	else if (GameGlobal::GetSpecialBulletType() == 3) {
		buttonSelect.SetPosition(cameraPos - Point(-8, -45));
	}
	buttonSelect.SetTranslation(trans);
	buttonSelect.SetScale(D3DXVECTOR2(0.5, 0.5));
	selection->Draw(buttonSelect);

}

void SceneBullet::Release()
{
}

SceneBullet::~SceneBullet()
{
	Release();
}
