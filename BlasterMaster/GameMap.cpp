#include "GameMap.h"
#include "Utils.h"
#include "GameGlobal.h"

GameMap::GameMap(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	LoadMap(filePath, texLib, spriteLib);
}

GameMap::~GameMap()
{
	Release();
}

void GameMap::LoadMap(const char* filePath, TextureLibrary* texLib, SpriteLibrary* spriteLib)
{
	mMap = new Tmx::Map();
	mMap->ParseFile(filePath);

	BoundingBox r;
	r.l = 0;
	r.t = 0;
	r.r = this->GetWidth();
	r.b = this->GetHeight();

	for (size_t i = 0; i < mMap->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = mMap->GetTileset(i);
		
		// convert str to wstr of tileset path
		std::string tileSetPathStr = tileset->GetImage()->GetSource();
		std::wstring tileSetPathWstr = std::wstring(tileSetPathStr.begin(), tileSetPathStr.end());

		// map sideview 
		texLib->Add(SIDEVIEW_TILE_SET_ID, tileSetPathWstr.c_str(), NULL);
		spriteLib->Add(SIDEVIEW_TILE_SET_ID, r, texLib->Get(SIDEVIEW_TILE_SET_ID));

		Sprite* sprite = spriteLib->Get(SIDEVIEW_TILE_SET_ID);


		mListTileset.insert(std::pair<int, Sprite*>(i, sprite));
	}
}

bool GameMap::isContain(BoundingBox rect1, BoundingBox rect2)
{
	if (rect1.l > rect2.r || rect1.r < rect2.l || rect1.t > rect2.b || rect1.b < rect2.t)
	{
		return false;
	}

	return true;
}

Tmx::Map* GameMap::GetMap()
{
	return mMap;
}

int GameMap::GetWidth()
{
	return mMap->GetWidth() * mMap->GetTileWidth();
}

int GameMap::GetHeight()
{
	return mMap->GetHeight() * mMap->GetTileHeight();
}

int GameMap::GetTileWidth()
{
	return mMap->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return mMap->GetTileHeight();
}

void GameMap::Draw()
{
	//LeSon
	Point trans = Point(GameGlobal::GetWidth() /2 - mCamera->GetPosition().x,
		GameGlobal::GetHeight() / 2 - mCamera->GetPosition().y);
	//Point trans = Point(-1024,-1024);

	for (size_t i = 0; i < mMap->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = mMap->GetTileLayer(i);

		if (!layer->IsVisible())
		{
			continue;
		}

		BoundingBox sourceRECT;

		int tileWidth = mMap->GetTileWidth();
		int tileHeight = mMap->GetTileHeight();

		int centertileX = mCamera->GetPosition().x / tileWidth;
		int centertileY = mCamera->GetPosition().y / tileHeight;
		
		for (size_t m = max(0,centertileY-10); m < min(centertileY+10,layer->GetHeight()); m++)
		//for (size_t m = 0; m < layer->GetHeight(); m++)
		{
			for (size_t n = max(0, centertileX - 10); n < min(centertileX + 10, layer->GetHeight()); n++)
			//for (size_t n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = mMap->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = mListTileset[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);

					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					sourceRECT.t = y * tileHeight;
					sourceRECT.b = sourceRECT.t + tileHeight;
					sourceRECT.l = x * tileWidth;
					sourceRECT.r = sourceRECT.l + tileWidth;

					//tru tilewidth/2 va tileheight/2 vi Sprite ve o vi tri giua hinh anh cho nen doi hinh de cho
					//dung toa do (0,0) cua the gioi thuc la (0,0) neu khong thi se la (-tilewidth/2, -tileheigth/2);
					Point position(n * tileWidth + tileWidth / 2, m * tileHeight + tileHeight / 2);

					// camera handling
					if (mCamera != NULL)
					{
						BoundingBox objRECT;
						objRECT.l = position.x - tileWidth / 2;
						objRECT.t = position.y - tileHeight / 2;
						objRECT.r = objRECT.l + tileWidth;
						objRECT.b = objRECT.t + tileHeight;

						//neu nam ngoai camera thi khong Draw
						if (isContain(objRECT, mCamera->GetBound()) == false)
							continue;
					}

					sprite->SetWidth(tileWidth);
					sprite->SetHeight(tileHeight);
					sprite->SetSourceRect(sourceRECT);

					DrawArguments drawArguments;
					drawArguments.SetPosition(position);
					drawArguments.SetTranslation(trans);
					
					sprite->Draw(drawArguments);
				}
			}
		}
	}
}

void GameMap::Release()
{
	if (mMap != nullptr)
		delete mMap;
}

void GameMap::SetCamera(Camera * camera)
{
	this->mCamera = camera;
}

bool GameMap::IsBoundLeft()
{
	return (mCamera->GetBound().l == 0);
}

bool GameMap::IsBoundRight()
{
	return (mCamera->GetBound().r == this->GetWidth());
}

bool GameMap::IsBoundTop()
{
	return (mCamera->GetBound().t == 0);
}

bool GameMap::IsBoundBottom()
{
	return (mCamera->GetBound().b == this->GetHeight());
}

BoundingBox GameMap::GetWorldMapBound()
{
	BoundingBox bound;
	bound.l = bound.t = 0;
	bound.r = mMap->GetWidth() * mMap->GetTileWidth();
	bound.b = mMap->GetHeight() * mMap->GetTileHeight();

	return bound;
}
