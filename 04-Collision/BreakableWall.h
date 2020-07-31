#pragma once
#include "GameObject.h"
#include "WallPiece.h"

#define NORMAL	0
#define BREAK	1
#define BREAKABLE_WALL_BBOX_WIDTH	32
#define BREAKABLE_WALL_BBOX_HEIGHT	32

class BreakableWall : public CGameObject
{
	bool isBreakToPieces = false;
	WallPieces* pieces = NULL;

public:
	BreakableWall();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObject);
	void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

