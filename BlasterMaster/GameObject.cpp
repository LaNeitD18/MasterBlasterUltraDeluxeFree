#include <d3dx9.h>
#include <algorithm>


#include "Utils.h"
#include "Textures.h"
#include "Game.h"
#include "GameObject.h"
#include "Sprites.h"

GameObject::GameObject()
{
	drawArguments.Init();
	v = Point();
	nx = 1;	
}

RESULT GameObject::Init(TextureLibrary * textureLib)
{
	bbox = textureLib->Get(ID_TEX_BBOX);
	return 0;
}

void GameObject::RenderBoundingBox()
{
	Point pos = drawArguments.GetPosition();
	D3DXVECTOR3 p(pos.x, pos.y, 0);
	RECT rect;

	BoundingBox box;

	GetBoundingBox(box);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)box.r - (int)box.l;
	rect.bottom = (int)box.b - (int)box.t;

	Game::GetInstance()->Draw(pos, bbox, rect, 32);
}

GameObject::~GameObject()
{

}
