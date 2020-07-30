#pragma once
#include "GameObject.h"

#define ZOMBIE_BBOX_WIDTH 32
#define ZOMBIE_BBOX_HEIGHT 63

#define ZOMBIE_STATE_WALK_LEFT	0
#define ZOMBIE_STATE_DIE		1
#define ZOMBIE_STATE_WALK_RIGHT	2

class Zombie : public CGameObject
{
public:
	Zombie(int state);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom);
	void SetState(int state);
};