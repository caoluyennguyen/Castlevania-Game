#pragma once
#include "GameObject.h"
class DownStair : public CGameObject
{
	int width;
	int height;
public:
	DownStair(float l, float t, float r, float b);
	int GetHeight() { return height; };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

