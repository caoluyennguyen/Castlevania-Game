#include "Fleaman.h"
#include "Ground.h"
#include "Simon.h"
#include "Ghost.h"

Fleaman::Fleaman() : CGameObject()
{
	this->jump = -1;
	this->isEnemy = true;
	this->isActive = true;
	this->isAbleToJump = true;
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
	vy += FLEAMAN_GRAVITY * dt;

	if (this->GetState() == FLEAMAN_STATE_DIE && animation_set->at(FLEAMAN_STATE_DIE)->isOver(200))
	{
		this->enable = false;
	}

	/*if (state != FLEAMAN_STATE_IDLE_RIGHT)
	{
		if (isAbleToJump)
		{
			this->SetState(FLEAMAN_STATE_JUMP_RIGHT);
			isAbleToJump = false;
		}
		else {
			isJumping = true;
		}
	}*/

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

			if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
			{
				if (ny != 0) {
					if (this->GetState() == FLEAMAN_STATE_IDLE_RIGHT) vx = vy = 0;
					else if (this->GetState() != FLEAMAN_STATE_DIE) {
						CalculateOrietation();
						this->SetState(FLEAMAN_STATE_JUMP_RIGHT);
					}
				}
				//jump *= -1;
				//isAbleToJump = true;
				//isJumping = false;
			}
			else if (dynamic_cast<Simon*>(e->obj) || dynamic_cast<Ghost*>(e->obj)) // if e->obj is Ground
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
		//this->isActive = false;
		animation_set->at(FLEAMAN_STATE_DIE)->resetAnimation();
		animation_set->at(FLEAMAN_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	case FLEAMAN_STATE_JUMP_RIGHT:
	{
		vx = nx * 0.2f;
		vy = -0.2f;
		break;
	}
	default:
		break;
	}
}

void Fleaman::CalculateOrietation()
{
	float dx = abs(x - Simon::GetInstance()->x);
	float dy = abs(y - Simon::GetInstance()->y);

	// int nx, ny;

	if (x < Simon::GetInstance()->x) {
		this->nx = 1;
		//state = FLEAMAN_STATE_JUMP_RIGHT;
	}
	else {
		this->nx = -1;
		//state = FLEAMAN_STATE_JUMP_LEFT;
	}

	/*if (y < Simon::GetInstance()->y)
		ny = 1;
	else
		ny = -1;

	if (dx < 10 && dy < 10)
	{
		vx = nx * dx / 250;
		vy = ny * dy / 250;
	}
	else
	{
		vx = nx * dx / 1000;
		vy = ny * dy / 1000;
	}*/
}