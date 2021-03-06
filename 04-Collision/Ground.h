#pragma once
#include "GameObject.h"
class Ground : public CGameObject
{
	int width;
	int height;
public:
	Ground(float l, float t, float r, float b);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
};

