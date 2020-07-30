#pragma once
#include "GameObject.h"

#define DAGGER_LEFT 0
#define DAGGER_RIGHT 1
#define AXE 2
#define BOOMERANG 3
#define HOLYWATER 4
#define FIRE 5
#define BONE 6

#define WEAPON_GRAVITY 0.001f
class Weapon : public CGameObject
{
	int nxFlyBack;
	float vxFlyBack;
	DWORD boomerangBack;
	DWORD renderHolywater;
	DWORD axeFalling;
public:
	int TypeOfWeapon;

	Weapon();

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void SetState(int state);

	void SetNxFlyBack(int nx) { this->nxFlyBack = -nx; }
	void SetVxFlyBack(float vx) { this->vxFlyBack = -vx; }
	void SetWeaponPosition(int simonX, int simonY, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	void StartRenderHolywater() { renderHolywater = GetTickCount(); };
	void StartBoomerangBack() { boomerangBack = GetTickCount(); };
	void StartAxeFalling() { axeFalling = GetTickCount(); };
};

