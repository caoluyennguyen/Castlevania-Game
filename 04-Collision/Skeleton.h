#pragma once
#include "GameObject.h"

#define SKELETON_BBOX_WIDTH		32
#define SKELETON_BBOX_HEIGHT	31

#define SKELETON_STATE_IDLE_LEFT	0
#define SKELETON_STATE_WALK_LEFT	1
#define SKELETON_STATE_DIE			2
#define SKELETON_STATE_IDLE_RIGHT	3

class Skeleton : public CGameObject
{
public:
	Skeleton();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};
