#pragma once
#include "GameObject.h"

#define WHIP_BBOX_WIDTH 55;
#define WHIP_BBOX_HEIGHT 15;
#define LONG_WHIP_BBOX_WIDTH 85;

class Whip : public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void SetWhipPosition(int simonX, int simonY, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);
};

