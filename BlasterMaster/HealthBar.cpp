//#include "HealthBar.h"
//#include "Utils.h"
//#include "GameGlobal.h"
//
//HealthBar::HealthBar()
//{
//	
//}
//
//HealthBar::HealthBar(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib)
//{
//	Load(filePath, texLib, spriteLib);
//}
//
//HealthBar::~HealthBar()
//{
//	Release();
//}
//
//void Load(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib)
//{
//	for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
//	{
//		const Tmx::Tileset *tileset = mMap->GetTileset(i);
//
//		// convert str to wstr of tileset path
//		std::string tileSetPathStr = tileset->GetImage()->GetSource();
//		std::wstring tileSetPathWstr = std::wstring(tileSetPathStr.begin(), tileSetPathStr.end());
//
//		// map sideview 
//		texLib->Add(SIDEVIEW_TILE_SET_ID, tileSetPathWstr.c_str(), NULL);
//		spriteLib->Add(SIDEVIEW_TILE_SET_ID, r, texLib->Get(SIDEVIEW_TILE_SET_ID));
//
//		Sprite* sprite = spriteLib->Get(SIDEVIEW_TILE_SET_ID);
//
//
//		mListTileset.insert(std::pair<int, Sprite*>(i, sprite));
//	}
//}
