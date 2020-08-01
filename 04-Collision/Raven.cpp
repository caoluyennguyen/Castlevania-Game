#include "Raven.h"
#include "Simon.h"

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
	CGameObject::Update(dt);
	x += dx;
	y += dy;

	if (state == RAVEN_STATE_DIE && animation_set->at(RAVEN_STATE_DIE)->isOver(300))
	{
		this->enable = false;
	}

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

	CalculateVelocity();
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
		vx = nx * dx / 250;
		vy = ny * dy / 250;
	}
	else
	{
		vx = nx * dx / 1000;
		vy = ny * dy / 1000;
	}
}