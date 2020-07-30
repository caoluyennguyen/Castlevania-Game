#pragma once
#include "GameObject.h"
#include "Simon.h"

#define BOSS_BBOX_WIDTH 96
#define BOSS_BBOX_HEIGHT 46
#define BOSS_BBOX_FLY_WIDTH 100
#define BOSS_BBOX_FLY_HEIGHT 150
#define BOSS_TIME_WAITING 1500

#define BOSS_STATE_IDLE		0
#define BOSS_STATE_ATTACK	1
#define BOSS_STATE_DIE		2
#define BOSS_STATE_FLY		3

class Boss : public CGameObject
{
	int hp;
	int score;

	int count = 0;
	Simon *simon;
	D3DXVECTOR2 point;

	DWORD waiting;
	bool isWaiting = true;
	bool isFlyToSimon;
	bool isFlyToPoint;
public:
	Boss(Simon *simon);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);

	void LostHp(int hp) { this->hp -= hp; }
	void StartWaiting() { this->isWaiting = true; this->waiting = GetTickCount(); }
	D3DXVECTOR2 GetRandomPoint();
	void CalculateVelocity();
	void FlyToPoint(DWORD dt);
};