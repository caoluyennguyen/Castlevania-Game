#pragma once
#include "GameObject.h"
#include "Weapon.h"

#define SKELETON_BBOX_WIDTH		32
#define SKELETON_BBOX_HEIGHT	64

#define SKELETON_STATE_IDLE	0
#define SKELETON_STATE_WALK	1
#define SKELETON_STATE_DIE	2

#define SKELETON_THROW_BONE_DELAY	2000

class Skeleton : public CGameObject
{
	int move = 1;
	bool isWaiting;
	DWORD waiting;
public:
	Weapon* weapon;
	bool isAbleToThrowBone;
	Skeleton(int nx);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
	void StartWaiting() { this->waiting = GetTickCount(); }
};
