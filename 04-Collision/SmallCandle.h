#pragma once
#include "GameObject.h"

#define SMALL_CANDLE_BBOX_WIDTH		16
#define SMALL_CANDLE_BBOX_HEIGHT	32

#define SMALL_CANDLE_STATE_NORMAL		0
#define SMALL_CANDLE_STATE_DESTROYED	1

class SmallCandle : public CGameObject
{
	int typeOfItem;
public:
	SmallCandle(int TypeOfItem);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);
};

