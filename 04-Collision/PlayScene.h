#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Simon.h"
#include "Ground.h"
#include "Candle.h"
#include "Whip.h"
#include "Item.h"
#include "Tilemap.h"
#include "Weapon.h"
#include "Grid.h"

class CPlayScene : public CScene
{
protected:
	Simon* player;					// A play scene has to have player, right? 
	//Whip* whip;
	Weapon* weapon;
	Tilemap* tilemap;
	Grid* grid;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> coObjects;

	void _ParseSection_TILEMAP(string line);
	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_GRID(string line);
public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

