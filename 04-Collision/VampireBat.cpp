#include "VampireBat.h"
#include "Textures.h"
#include "Game.h"

VampireBat::VampireBat() : CGameObject()
{
	vx = vy = 0;
	isEnemy = true;
	timeToFly = GetTickCount();
	maxHeightToFly = x + 96;

	SetState(VAMPIREBAT_STATE_IDLE);
}

void VampireBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + VAMPIREBAT_BBOX_WIDTH;
	bottom = y + VAMPIREBAT_BBOX_HEIGHT;
}

void VampireBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->GetState() != VAMPIREBAT_STATE_DIE)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);
		x += dx;
		y += dy;

		if (y >= maxHeightToFly)
		{
			this->vy = 0;
		}
		else if (this->GetState() != VAMPIREBAT_STATE_IDLE) this->vy = 0.1f;

		if (isAbleToFly)
		{
			this->SetState(VAMPIREBAT_STATE_FLY_RIGHT);
		}
	}
	else {
		if (animation_set->at(VAMPIREBAT_STATE_DIE)->isOver(300))
		{
			this->enable = false;
		}
	}
}

void VampireBat::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void VampireBat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case VAMPIREBAT_STATE_FLY_LEFT:
		vx = -0.2f;
		break;
	case VAMPIREBAT_STATE_FLY_RIGHT:
		vx = 0.2f;
		break;
	case VAMPIREBAT_STATE_DIE:
		vx = vy = 0;
		animation_set->at(VAMPIREBAT_STATE_DIE)->resetAnimation();
		animation_set->at(VAMPIREBAT_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void VampireBat::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 250;
	bottom = y + 100;
}
