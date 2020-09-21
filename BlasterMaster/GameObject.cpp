#include <d3dx9.h>

#include "GameObject.h"
#include "Game.h"

GameObject::GameObject(LPCWSTR texturePath) {
	x = y = 100;
	texture = Game::GetInstance()->LoadTexture(texturePath);
}

void GameObject::Update(DWORD dt) {

}

void GameObject::Render() {
	Game::GetInstance()->Draw(x, y, texture);
}

GameObject::~GameObject() {
	if (texture != NULL) texture->Release();
}
