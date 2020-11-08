#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Game.h"
#include "Utils.h"
#include "useful_stuff.h"

#include "SceneArea2SideView.h"
#include "SceneArea2Overhead.h"

Game * Game::__instance = NULL;

/*
	Initialize DirectX, create a Direct3D device for rendering within the window, initial Sprite library for 
	rendering 2D images
	- hInst: Application instance handle
	- hWnd: Application window handle
*/

static BoundingBox cameraLimitAreaOfSection[15] = {
	// section A
	BoundingBox(0, 2814, 1038, 3094),
	// section B
	BoundingBox(1024, 1792, 1550, 3094),
	//section C
	BoundingBox(1536, 1792, 2062, 2072),
	// section D
	BoundingBox(2048, 1024, 2574, 2072),
	// section E
	BoundingBox(2560, 1792, 3086, 2072),
	BoundingBox(1536, 32, 2062, 1814),
};

RESULT Game::Init(HWND hWnd)
{
	LPDIRECT3D9 d3d = Direct3DCreate9(D3D_SDK_VERSION);

	this->hWnd = hWnd;									

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;

	RECT r;
	GetClientRect(hWnd, &r);	// retrieve Window width & height 

	d3dpp.BackBufferHeight = r.bottom + 1;
	d3dpp.BackBufferWidth = r.right + 1;

	screen_height = r.bottom + 1;
	screen_width = r.right + 1;

	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddv);
	GameGlobal::SetCurrentDevice(d3ddv);

	if (d3ddv == NULL)
	{
		OutputDebugString(L"[ERROR] CreateDevice failed\n");
		return 1;
	}

	d3ddv->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);

	// Initialize sprite helper from Direct3DX helper library
	D3DXCreateSprite(d3ddv, &spriteHandler);
	GameGlobal::SetCurrentSpriteHandler(spriteHandler);

	OutputDebugString(L"[INFO] InitGame done;\n");

	return 0;
}

/*
	Utility function to wrap LPD3DXSPRITE::Draw 
*/
void Game::Draw(Point pos, LPDIRECT3DTEXTURE9 texture, RECT rect, int alpha)
{
	Point drawPos = pos - cameraPosition;
	D3DXVECTOR3 pCenter(abs(rect.left - rect.right) / 2, abs(rect.bottom - rect.top) / 2, 0);
	D3DXMATRIX flipMatrix;
	D3DXMatrixScaling(&flipMatrix, 1, 1, 1);
	D3DXMATRIX transposeMatrix;
	D3DXMatrixTranslation(&transposeMatrix, drawPos.x, drawPos.y, 0);
	D3DXMATRIX worldMatrix;
	worldMatrix = flipMatrix * transposeMatrix;
	spriteHandler->SetTransform(&worldMatrix);
	spriteHandler->Draw(texture, &rect, &pCenter, NULL, D3DCOLOR_ARGB(alpha, 255, 255, 255));
}

Game::~Game()
{
	DESTROY(spriteHandler);
	DESTROY(backBuffer);
	DESTROY(d3ddv);
	DESTROY(d3d);
	DESTROY(input);
}

Game::Game()
{
	input = new Input();
	input->Initialize();
	GameGlobal::SetInput(input);
}

void Game::Render()
{
	LPDIRECT3DDEVICE9 d3ddv = GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = GetBackBuffer();
	LPD3DXSPRITE spriteHandler = GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		GetCurrentScene()->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}

/*
	Standard sweptAABB implementation
	Source: GameDev.net
*/
void Game::SweptAABB(
	float ml, float mt,	float mr, float mb,			
	float dx, float dy,			
	float sl, float st, float sr, float sb,
	float &t, float &nx, float &ny)
{

	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry; 
	float t_exit; 

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float bt = dy > 0 ? mt : mt + dy;
	float br = dx > 0 ? mr + dx : mr;
	float bb = dy > 0 ? mb + dy : mb;

	if (br < sl || bl > sr || bb < st || bt > sb) return;


	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0)
	{
		dx_entry = sl - mr; 
		dx_exit = sr - ml;
	}
	else if (dx < 0)
	{
		dx_entry = sr - ml;
		dx_exit = sl- mr;
	}


	if (dy > 0)
	{
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}
	else if (dy < 0)
	{
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}

	if (dx == 0)
	{
		tx_entry = -999999.0f;
		tx_exit = 999999.0f;
	}
	else
	{
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}
	
	if (dy == 0)
	{
		ty_entry = -99999.0f;
		ty_exit = 99999.0f;
	}
	else
	{
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}
	

	if (  (tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);
	
	if (t_entry > t_exit) return; 

	t = t_entry; 

	if (tx_entry > ty_entry)
	{
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else 
	{
		nx = 0.0f;
		dy > 0?ny = -1.0f:ny = 1.0f;
	}

}

Game *Game::GetInstance()
{
	if (__instance == NULL) __instance = new Game();
	return __instance;
}

#define MAX_GAME_LINE 1024


#define GAME_FILE_SECTION_UNKNOWN -1
#define GAME_FILE_SECTION_SETTINGS 1
#define GAME_FILE_SECTION_SCENES 2

void Game::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	if (tokens[0] == "start")
		current_scene = atoi(tokens[1].c_str());
	else
		DebugOut(L"[ERROR] Unknown game setting %s\n", ToWSTR(tokens[0]).c_str());
}

void Game::_ParseSection_SCENES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return;
	int id = atoi(tokens[0].c_str());
	LPCWSTR path = ToLPCWSTR(tokens[1]);

	Scene* scene = new SceneArea2SideView(id, path, this, Point(screen_width, screen_height)); // start map
	scenes[id] = scene;
}

/*
	Load game campaign file and load/initiate first scene
*/
void Game::Init(LPCWSTR gameFile)
{
	DebugOut(L"[INFO] Start loading game file : %s\n", gameFile);

	ifstream f;
	f.open(gameFile);
	char str[MAX_GAME_LINE];

	// current resource section flag
	int section = GAME_FILE_SECTION_UNKNOWN;

	while (f.getline(str, MAX_GAME_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") { section = GAME_FILE_SECTION_SETTINGS; continue; }
		if (line == "[SCENES]") { section = GAME_FILE_SECTION_SCENES; continue; }

		//
		// data section
		//
		switch (section)
		{
			case GAME_FILE_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case GAME_FILE_SECTION_SCENES: _ParseSection_SCENES(line); break;
		}
	}
	f.close();

	DebugOut(L"[INFO] Loading game file : %s has been loaded successfully\n",gameFile);

	SwitchScene(current_scene);
}

void Game::SwitchScene(int scene_id)
{
	DebugOut(L"[INFO] Switching to scene %d\n", scene_id);
	DebugOut(L"[INFO] Ala %d\n", scenes[current_scene]);
	
	// map release sucks, must be fixed to switch properly hihi
	//scenes[current_scene]->Release();
	
	current_scene = scene_id;
	Scene* s = scenes[scene_id];
	
	s->Init();
}

