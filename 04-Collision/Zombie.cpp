#include "Zombie.h"
#include "Ground.h"

Zombie::Zombie(int state) : CGameObject()
{
	isActive = false;
	this->isEnemy = true;
	//this->isActive = true;
	SetState(state);
}

void Zombie::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ZOMBIE_BBOX_WIDTH;
	bottom = y + ZOMBIE_BBOX_HEIGHT;
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->GetState() != ZOMBIE_STATE_DIE)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);
		vy += 0.01f;

		if (isActive) {
			if (state == ZOMBIE_STATE_WALK_LEFT) vx = -0.1f;
			else vx = 0.1f;
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
					if (e->ny != 0) vy = 0;
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
	else {
		if (animation_set->at(ZOMBIE_STATE_DIE)->isOver(300))
		{
			this->enable = false;
		}
	}
}

void Zombie::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void Zombie::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ZOMBIE_STATE_WALK_LEFT:
		break;
	case ZOMBIE_STATE_WALK_RIGHT:
		break;
	case ZOMBIE_STATE_DIE:
		vx = vy = 0;
		animation_set->at(ZOMBIE_STATE_DIE)->resetAnimation();
		animation_set->at(ZOMBIE_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Zombie::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 100;
	top = y - 100;
	right = left + 10;
	bottom = top + 200;
}
