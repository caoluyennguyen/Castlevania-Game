#pragma once
#include "GameObject.h"

#define VAMPIREBAT_BBOX_WIDTH 33
#define VAMPIREBAT_BBOX_HEIGHT 31

#define VAMPIREBAT_STATE_IDLE				0
#define VAMPIREBAT_STATE_FLY_RIGHT			1
#define VAMPIREBAT_STATE_INJURED_RIGHT		2
#define VAMPIREBAT_STATE_DIE				3
#define VAMPIREBAT_STATE_FLY_LEFT			4
#define VAMPIREBAT_STATE_INJURED_LEFT		5

class VampireBat : public CGameObject
{
	DWORD timeToFly;
	int maxHeightToFly;
	bool isAbleToFly = false;
public:
	VampireBat();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	void StartTimeToFly() { timeToFly = GetTickCount(); };
	void SetAbleToFly() { isAbleToFly = true; };
};
