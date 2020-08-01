#pragma once
#include "d3dx9.h"
#include "Define.h"
#include "Textures.h"
#include "Sprites.h"
#include "Utils.h"
#include "PlayScene.h"

#define FILEPATH_FONT L"Font\\prstart.ttf"
#define TIME_PLAY 1000

class HeadUpDisplay
{
private:
	static HeadUpDisplay* __instance;
	int x, y, score, time, stage, weaponId;
	int simonHp, bossHp, simonHeart, scene;
	string str_score, str_time, str_simonHeart, str_scene;

	CSprite* current, * lose, * boss, * weapon;
	vector<LPSPRITE> currentHP, loseHP, bossHP;
	vector<LPSPRITE> weaponIcon;

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
	void SetWeapon(int weapon) { this->weaponId = weapon; }
	void SetHeart(int heart) { this->simonHeart = heart; }
};

