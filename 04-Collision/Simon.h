#pragma once
#include "GameObject.h"
#include "Define.h"
#include "Whip.h"

class Simon : public CGameObject
{
	int weapon = 0;
	int nxUpStair, nxDownStair;
	int distanceWalkOnStair = 0;
	bool isOnGround = false;
	DWORD untouchable_start;
public:
	Whip *whip;
	int whip_level; // 0: normal, 2: chain, 4 long chain
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

	Simon() : CGameObject()
	{
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

};