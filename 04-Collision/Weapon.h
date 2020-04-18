#pragma once
#include "GameObject.h"

class Weapon : public CGameObject
{
public:
	Weapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	void SetWeaponPosition(int simonX, int simonY, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
};

