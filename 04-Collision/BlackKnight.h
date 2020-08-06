#pragma once
#include "Enemy.h"

#define BLACKKNIGHT_BBOX_WIDTH 32
#define BLACKKNIGHT_BBOX_HEIGHT 64

#define BLACKKNIGHT_STATE_IDLE_LEFT		0
#define BLACKKNIGHT_STATE_WALK_LEFT		1
#define BLACKKNIGHT_STATE_IDLE_RIGHT	2
#define BLACKKNIGHT_STATE_WALK_RIGHT	3
#define BLACKKNIGHT_STATE_DIE			4

class BlackKnight : public Enemy
{
	int left, right, minX, maxX;
public:
	BlackKnight(int minX, int maxX);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState(int state);
};

