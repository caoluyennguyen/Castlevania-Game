#include "Skeleton.h"
#include "Ground.h"
#include "Simon.h"

Skeleton::Skeleton() : CGameObject()
{
	this->isEnemy = true;
	SetState(SKELETON_STATE_WALK_LEFT);
}

void Skeleton::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + SKELETON_BBOX_WIDTH;
	bottom = y + SKELETON_BBOX_HEIGHT;
}

void Skeleton::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->GetState() != SKELETON_STATE_DIE)
	{
		// Calculate dx, dy 
		CGameObject::Update(dt);

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
					this->SetState(SKELETON_STATE_WALK_LEFT);
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
		if (animation_set->at(SKELETON_STATE_DIE)->isOver(300))
		{
			this->enable = false;
		}
	}
}

void Skeleton::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
	RenderActiveBoundingBox();
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SKELETON_STATE_IDLE_LEFT:
		vx = vy = 0;
		break;
	case SKELETON_STATE_WALK_LEFT:
		//vx = -0.1f;
		break;
	case SKELETON_STATE_IDLE_RIGHT:
		//vx = 0.1f;
		break;
	case SKELETON_STATE_DIE:
		vx = vy = 0;
		animation_set->at(SKELETON_STATE_DIE)->resetAnimation();
		animation_set->at(SKELETON_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Skeleton::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - 100;
	top = y - 100;
	right = left + 10;
	bottom = top + 600;
}