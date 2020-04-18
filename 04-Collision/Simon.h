#pragma once
#include "GameObject.h"
#include "Define.h"

class Simon : public CGameObject
{
	int weapon = 0;

	bool isOnGround = false;
	DWORD untouchable_start;
public:
	int untouchable;
	bool isStand = true;
	bool isThrowWeapon = false;
	Simon() : CGameObject()
	{
		untouchable = 0;
		SetState(SIMON_STATE_IDLE_RIGHT);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	//virtual void LoadResource();
	virtual void Render();
	void SetState(int state);
	void StartUntouchable();

	bool CheckStandGround() { return isOnGround; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};