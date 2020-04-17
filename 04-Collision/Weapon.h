#pragma once
#include "GameObject.h"

class Weapon : public CGameObject
{
public:
	Weapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void SetState(int state);

	void SetWeaponPosition(int simonX, int simonY, bool isStand);
};

