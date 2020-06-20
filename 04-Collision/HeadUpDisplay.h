#pragma once
#include "d3dx9.h"
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "PlayScene.h"

#define FILEPATH_FONT L"Font\\prstart.ttf"

class HeadUpDisplay
{
private:
	static HeadUpDisplay* __instance;
	int x, y, score, time;

	CSprite* current, * lose, * boss, * weapon;
	vector<LPSPRITE> currentHP, loseHP, bossHP;

	CGame* game;
	string headUpDetails;
	ID3DXFont* font;
public:
	HeadUpDisplay() {};
	HeadUpDisplay(CGame* game);
	void LoadResource();
	void UnLoadResource();
	void Update(DWORD dt);
	void Render();

	static HeadUpDisplay* GetInstance(CGame* game);
};

