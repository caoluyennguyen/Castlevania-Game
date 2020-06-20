#pragma once
#include "GameObject.h"

#define BLACKKNIGHT_BBOX_WIDTH 32
#define BLACKKNIGHT_BBOX_HEIGHT 64

#define BLACKKNIGHT_STATE_NORMAL	0
#define BLACKKNIGHT_STATE_WALK	1

class BlackKnight : public CGameObject
{
public:
	BlackKnight();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};

