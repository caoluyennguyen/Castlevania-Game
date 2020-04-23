#pragma once
#include "GameObject.h"
class Item : public CGameObject
{
	int typeOfItem;
public:
	Item(int type);

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
};

