#pragma once
#include "GameObject.h"

#define ITEM_GRAVITY	0.001f
#define ITEM_SMALLHEART	1

class Item : public CGameObject
{
	//int typeOfItem;
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

