#include "Ghost.h"

Ghost::Ghost() : CGameObject()
{
	this->fly = 1;
	this->isEnemy = true;
	this->isActive = false;
	SetState(GHOST_STATE_IDLE);
}

void Ghost::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GHOST_BBOX_WIDTH;
	bottom = y + GHOST_BBOX_HEIGHT;
}

void Ghost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->GetState() != GHOST_STATE_DIE)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		if (this->GetState() == GHOST_STATE_FLY_LEFT)
		{
			vy += fly * 0.002f;
			if (vy > 0.05f) fly = -1;
			else if (vy < -0.05f) fly = 1;
		}
	}
	else {
		if (animation_set->at(GHOST_STATE_DIE)->isOver(200))
		{
			this->enable = false;
		}
	}
}

void Ghost::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	//RenderBoundingBox();
	//RenderActiveBoundingBox();
}

void Ghost::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GHOST_STATE_IDLE:
		vx = vy = 0;
		break;
	case GHOST_STATE_FLY_LEFT:
		vx = -0.1f;
		break;
	case GHOST_STATE_FLY_RIGHT:
		vx = 0.1f;
		break;
	case GHOST_STATE_DIE:
		vx = vy = 0;
		//this->isActive = false;
		animation_set->at(GHOST_STATE_DIE)->resetAnimation();
		animation_set->at(GHOST_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Ghost::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 250;
	top = y - 50;
	right = left + 80;
	bottom = top + 100;
}