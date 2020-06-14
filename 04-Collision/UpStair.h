#pragma once
#include "GameObject.h"
class UpStair : public CGameObject
{
	int width;
	int height;
public:
	UpStair(float l, float t, float r, float b, float nx);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

