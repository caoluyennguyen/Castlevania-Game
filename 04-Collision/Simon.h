#pragma once
#include "GameObject.h"
#include "Define.h"
#include "Whip.h"

class Simon : public CGameObject
{
	int score;
	int heart;
	int playerHP;
	int weapon = 0;
	int nxUpStair, nxDownStair;
	int distanceWalkOnStair = 0;
	bool isOnGround = false;
	bool isTouchUpStair = false;
	bool isTouchDownStair = false;
	bool isFightingBoss = false;
	DWORD untouchable_start;
	static Simon* __instance;
public:
	Whip* whip = NULL;
	int whip_level; // 0: normal, 2: chain, 4: long chain
	int untouchable;
	int distance = 0;
	bool isStand = true;
	bool isStandOnElevator = false;
	bool isThrowWeapon = false;
	bool isAbleToStepUpStair = false;
	bool isAbleToStepDownStair = false;
	bool isStepOnStair = false;
	bool isAbleToStandOnStair = false;
	bool isStandUpStair = false;
	bool isStandDownStair = false;
	bool isAbleToMoveToStair = false;
	bool isMoveToStair = false;
	DWORD auto_start;

	static Simon* GetInstance();
	Simon() : CGameObject()
	{
		playerHP = 16;
		heart = 16;
		score = 0;

		untouchable = 0;
		whip = new Whip();
		whip_level = 0;
		SetState(SIMON_STATE_IDLE_RIGHT);
	}

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	//virtual void LoadResource();
	virtual void Render();
	
	void SetNxUpStair(int nxUpStair) { this->nxUpStair = nxUpStair; }
	int GetNxUpStair() { return this->nxUpStair; }
	void SetNxDownStair(int nxDownStair) { this->nxDownStair = nxDownStair; }
	int GetNxDownStair() { return this->nxDownStair; }
	void SetState(int state);
	void StartUntouchable();

	bool CheckStandGround() { return isOnGround; }
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	int GetScore() { return this->score; }
	void AddScore(int score) { this->score += score; }
	int GetHeart() { return this->heart; }
	void AddHeart(int heart) { this->heart += heart; }
	void SubtractHeart(int heart) { this->heart -= heart; }
	int GetPlayerHP() { return playerHP; }
	void AddPlayerHP(int playerHP) { this->playerHP += playerHP; }
	void SubtractPlayerHP(int playerHP) { this->playerHP -= playerHP; if (this->playerHP < 0) this->playerHP = 0; }
	int GetWeapon() { return weapon; }
	void SetWeapon(int weapon) { this->weapon = weapon; }
	bool IsFightingBoss() { return this->isFightingBoss; }

	void Unload() { whip = new Whip(); }
};