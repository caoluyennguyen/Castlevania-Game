#include "Boss.h"

Boss::Boss() : CGameObject()
{
	this->isEnemy = true;
	SetState(BOSS_STATE_IDLE);
}

void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOSS_BBOX_WIDTH;
	bottom = y + BOSS_BBOX_HEIGHT;
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->GetState() != BOSS_STATE_DIE)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);
		x += dx;
		y += dy;
	}
	else {
		if (animation_set->at(BOSS_STATE_DIE)->isOver(300))
		{
			this->enable = false;
		}
	}
}

void Boss::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void Boss::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOSS_STATE_IDLE:
		vx = vy = 0;
		break;
	case BOSS_STATE_DIE:
		vx = vy = 0;
		animation_set->at(BOSS_STATE_DIE)->resetAnimation();
		animation_set->at(BOSS_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Boss::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 100;
	top = y;
	right = x - 90;
	bottom = y + 800;
}
