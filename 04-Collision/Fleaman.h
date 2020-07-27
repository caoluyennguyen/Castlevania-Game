#pragma once
#include "GameObject.h"

#define FLEAMAN_BBOX_WIDTH 30
#define FLEAMAN_BBOX_HEIGHT 31

#define FLEAMAN_GRAVITY 0.001f

#define FLEAMAN_STATE_IDLE_RIGHT	0
#define FLEAMAN_STATE_JUMP_RIGHT	1
#define FLEAMAN_STATE_DIE			2

class Fleaman : public CGameObject
{
	DWORD start_jump;
	int jump = -1;
public:
	Fleaman();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	void StartJump() { start_jump = GetTickCount(); }
};