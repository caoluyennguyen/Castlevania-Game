#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_WIDTH 32
#define CANDLE_BBOX_HEIGHT 64

#define CANDLE_STATE_NORMAL		0
#define CANDLE_STATE_DESTROYED	1

class Candle : public CGameObject
{
public:
	Candle();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};

