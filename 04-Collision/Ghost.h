#pragma once
#include "GameObject.h"

#define GHOST_BBOX_WIDTH 32
#define GHOST_BBOX_HEIGHT 31

#define GHOST_STATE_FLY_LEFT	0
#define GHOST_STATE_DIE			1
#define GHOST_STATE_IDLE		2
#define GHOST_STATE_FLY_RIGHT	3

class Ghost : public CGameObject
{
	int fly;
public:
	Ghost();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};
