#pragma once
#include "GameObject.h"

#define RAVEN_BBOX_WIDTH	32
#define RAVEN_BBOX_HEIGHT	32

#define RAVEN_STATE_IDLE		0
#define RAVEN_STATE_FLY_LEFT	1
#define RAVEN_STATE_DIE			2
#define RAVEN_STATE_FLY_RIGHT	3

class Raven : public CGameObject
{
	int hp;
	DWORD startWaiting;
	DWORD endWaiting;
	bool isWaiting = false;
public:
	Raven();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	void LoseHp(int hp) { this->hp -= hp; }
	void StartWaiting() { isWaiting = true; startWaiting = GetTickCount(); }
	void CalculateVelocity();
};

