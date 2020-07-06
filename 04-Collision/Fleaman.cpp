#include "Fleaman.h"
#include "Ground.h"
#include "Simon.h"

Fleaman::Fleaman() : CGameObject()
{
	this->isEnemy = true;
	SetState(FLEAMAN_STATE_IDLE_RIGHT);
}

void Fleaman::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FLEAMAN_BBOX_WIDTH;
	bottom = y + FLEAMAN_BBOX_HEIGHT;
}

void Fleaman::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 195;
	top = y;
	right = x + 200;
	bottom = y + 600;
}

void Fleaman::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (this->GetState() == FLEAMAN_STATE_DIE && animation_set->at(FLEAMAN_STATE_DIE)->isOver(600))
	{
		this->enable = false;
	}

	if (this->GetState() == FLEAMAN_STATE_JUMP_RIGHT)
	{
		vy += jump * 0.01f;
		if (vy < -0.025f) jump = 1;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
			{
				this->SetState(FLEAMAN_STATE_IDLE_RIGHT);
			}
			if (dynamic_cast<Simon*>(e->obj)) // if e->obj is Ground
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else {
				x += dx;
				y += dy;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Fleaman::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void Fleaman::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FLEAMAN_STATE_IDLE_RIGHT:
		vx = vy = 0;
		break;
	case FLEAMAN_STATE_DIE:
		vx = vy = 0;
		animation_set->at(FLEAMAN_STATE_DIE)->resetAnimation();
		animation_set->at(FLEAMAN_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	case FLEAMAN_STATE_JUMP_RIGHT:
	{
		start_jump = GetTickCount();
		vx = 0.2f;
		vy = -0.02f;
		break;
	}
	default:
		break;
	}
}