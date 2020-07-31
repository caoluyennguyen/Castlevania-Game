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
	int x, y, score, time, stage;
	int simonHp, bossHp, simonHeart, scene;

	CSprite* current, * lose, * boss, * weapon;
	vector<LPSPRITE> currentHP, loseHP, bossHP;

	CGame* game;
	string headUpDetails;
	ID3DXFont* font;
public:
	HeadUpDisplay();
	HeadUpDisplay(CGame* game);
	void LoadResource();
	void UnLoadResource();
	void Update(DWORD dt);
	void Render();

	static HeadUpDisplay* GetInstance();

	void AddScore(int score) { this->score += score; }
	void SetStage(int stage) { this->stage = stage; }

	void SetSimonHP(int hp) { this->simonHp = hp; }
	void SetBossHP(int hp) { this->bossHp = hp; }
	void SetScene(int scene) { this->scene = scene; }
};

