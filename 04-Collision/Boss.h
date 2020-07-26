#pragma once
#include "GameObject.h"

#define BOSS_BBOX_WIDTH 96
#define BOSS_BBOX_HEIGHT 46

#define BOSS_STATE_IDLE		0
#define BOSS_STATE_ATTACK	1
#define BOSS_STATE_DIE		2

class Boss : public CGameObject
{
public:
	Boss();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};