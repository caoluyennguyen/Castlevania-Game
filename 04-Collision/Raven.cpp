#include "Raven.h"

Raven::Raven() : CGameObject()
{
	this->isEnemy = true;
	this->isActive = true;
	SetState(RAVEN_STATE_IDLE);
}

void Raven::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + RAVEN_BBOX_WIDTH;
	bottom = y + RAVEN_BBOX_HEIGHT;
}

void Raven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (this->GetState() != RAVEN_STATE_DIE)
	//{
	//	// Calculate dx, dy 
	//	CGameObject::Update(dt);
	//	x += dx;
	//	y += dy;

	//	if (this->GetState() == RAVEN_STATE_FLY_LEFT)
	//	{
	//		vy += fly * 0.002f;
	//		if (vy > 0.05f) fly = -1;
	//		else if (vy < -0.05f) fly = 1;
	//	}
	//}
	//else {
	//	if (animation_set->at(RAVEN_STATE_DIE)->isOver(300))
	//	{
	//		this->enable = false;
	//	}
	//}
}

void Raven::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void Raven::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case RAVEN_STATE_IDLE:
		vx = vy = 0;
		break;
	case RAVEN_STATE_FLY_LEFT:
		//vx = -0.1f;
		break;
	case RAVEN_STATE_FLY_RIGHT:
		//vx = 0.1f;
		break;
	case RAVEN_STATE_DIE:
		vx = vy = 0;
		animation_set->at(RAVEN_STATE_DIE)->resetAnimation();
		animation_set->at(RAVEN_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Raven::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 190;
	top = y;
	right = left - 10;
	bottom = top + 600;
}