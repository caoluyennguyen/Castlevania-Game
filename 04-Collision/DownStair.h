#pragma once
#include "GameObject.h"
class DownStair : public CGameObject
{
	int width;
	int height;
public:
	DownStair(float l, float t, float r, float b, float nx);
	int GetHeight() { return height; };
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
};

