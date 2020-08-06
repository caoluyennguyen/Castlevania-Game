#pragma once
#include "GameObject.h"

#define ITEM_DAGGER 0
#define ITEM_AXE 1
#define ITEM_HEART 2
#define ITEM_CHAIN 3
#define ITEM_BOOMERANG 4
#define ITEM_MONEY 5
#define ITEM_SMALLHEART 6
#define ITEM_TRIPPLESHOT 7
#define ITEM_HOLYWATER 8
#define ITEM_INVISIBILITYPOTION 9
#define ITEM_CROWN 10
#define ITEM_DOUBLESHOT 11
#define ITEM_ONEPIECE 12

#define ITEM_GRAVITY	0.0005f
#define ITEM_SMALLHEART	6
#define ITEM_SMALLHEART_VX	0.01f
#define ITEM_SMALLHEART_VX_MAX	0.09f
#define ITEM_SMALLHEART_VX_MIN	-0.09f

class Item : public CGameObject
{
	int alpha;
	DWORD timeDisappear;
	bool isShake = true;
	int shake = 1;
	float gravity = ITEM_GRAVITY;
public:
	Item(int type);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	void StartTimeDisappear() { timeDisappear = GetTickCount(); };
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
	void SetState(int state);
};

