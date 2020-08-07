#pragma once
#include "GameObject.h"
class UpStair : public CGameObject
{
	int width;
	int height;
public:
	UpStair(float l, float t, float r, float b, float nx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
};

