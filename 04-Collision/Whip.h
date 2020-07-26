#pragma once
#include "GameObject.h"

#define WHIP_BBOX_WIDTH 55;
#define WHIP_BBOX_HEIGHT 15;
#define LONG_CHAIN_BBOX_WIDTH 85;

#define STATE_WHIP_NORMAL_LEFT		0
#define STATE_WHIP_NORMAL_RIGHT		1
#define STATE_CHAIN_NORMAL_LEFT		2
#define STATE_CHAIN_NORMAL_RIGHT	3
#define STATE_CHAIN_LONG_RIGHT		4
#define STATE_CHAIN_LONG_RIGHT		5

class Whip : public CGameObject
{
	DWORD isRender;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void GetActiveBoundingBox(float& left, float& top, float& right, float& bottom) {};

	void SetWhipPosition(int simonX, int simonY, bool isStand);
	bool CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom);

	void StartHit();
};

