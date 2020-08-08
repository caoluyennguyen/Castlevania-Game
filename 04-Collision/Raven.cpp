#include "Raven.h"
#include "Simon.h"

Raven::Raven() : CGameObject()
{
	this->isEnemy = true;
	this->isActive = false;
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
	CGameObject::Update(dt);
	/*x += dx;
	y += dy;*/

	if (state == RAVEN_STATE_DIE) {
		if (animation_set->at(RAVEN_STATE_DIE)->isOver(300))
		{
			this->enable = false;
		}
		return;
	}

	if (isActive) {

		if (!isWaiting)
		{
			if (GetTickCount() - endWaiting > 1000)
			{
				StartWaiting();
			}
		}
		else {
			if (GetTickCount() - startWaiting > 1000) {
				isWaiting = false;
				endWaiting = GetTickCount();
			}
			else {
				vx = vy = 0;
				return;
			}
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
			x += min_tx * dx + nx * 0.4f;	// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Simon*>(e->obj)) // if e->obj is Ground
				{
					this->SetState(RAVEN_STATE_DIE);
					return;
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

	CalculateVelocity();
}

void Raven::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	//RenderBoundingBox();
	//RenderActiveBoundingBox();
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
		isActive = true;
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
	left = x - 150;
	top = y;
	right = left - 10;
	bottom = top + 600;
}

void Raven::CalculateVelocity()
{
	float dx = abs(x - Simon::GetInstance()->x);
	float dy = abs(y - Simon::GetInstance()->y);

	int nx, ny;

	if (x < Simon::GetInstance()->x) {
		nx = 1;
		state = RAVEN_STATE_FLY_RIGHT;
	}
	else {
		nx = -1;
		state = RAVEN_STATE_FLY_LEFT;
	}

	if (y < Simon::GetInstance()->y)
		ny = 1;
	else
		ny = -1;

	if (dx < 10 && dy < 10)
	{
		vx = nx * dx / 500;
		vy = ny * dy / 500;
	}
	else
	{
		vx = nx * dx / 1000;
		vy = ny * dy / 1000;
	}
}