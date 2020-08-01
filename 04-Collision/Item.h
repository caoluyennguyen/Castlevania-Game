#pragma once
#include "GameObject.h"

#define DAGGER 0
#define AXE 1
#define HEART 2
#define CHAIN 3
#define BOOMERANG 4
#define MONEY 5
#define SMALLHEART 6
#define TRIPPLESHOT 7
#define HOLYWATER 8

#define ITEM_GRAVITY	0.0005f
#define ITEM_SMALLHEART	6
#define ITEM_SMALLHEART_VX	0.01f
#define ITEM_SMALLHEART_VX_MAX	0.09f
#define ITEM_SMALLHEART_VX_MIN	-0.09f

class Item : public CGameObject
{
	//int typeOfItem;
	bool isShake = true;
	int shake = 1;
	float gravity = ITEM_GRAVITY;
public:
	Item(int type);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	//int GetTypeOfItem() { return typeOfItem; };
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void SetState(int state);
};

