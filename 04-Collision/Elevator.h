#pragma once
#include "GameObject.h"

#define ELEVATOR_BBOX_WIDTH 64
#define ELEVATOR_BBOX_HEIGHT 16

#define ELEVATOR_STATE_NORMAL	0

class Elevator : public CGameObject
{
	int minX, maxX;
public:
	Elevator(int minX, int maxX);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);
};

