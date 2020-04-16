#pragma once
#include "GameObject.h"
#include "Define.h"

class Simon : public CGameObject
{
	int untouchable;
	bool isOnGround = false;
	bool isStand = true;
	DWORD untouchable_start;
public:
	Simon() : CGameObject()
	{
		untouchable = 0;
		SetState(SIMON_STATE_IDLE_RIGHT);
	}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	//virtual void LoadResource();
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

	bool CheckStandGround() { return isOnGround; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};