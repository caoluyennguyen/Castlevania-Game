#pragma once
#include "GameObject.h"

class Effect : public CGameObject
{
public:
	Effect();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void SetState();
};

