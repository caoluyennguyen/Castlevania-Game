#include "Boss.h"
#include "Simon.h"
#include "Whip.h"
#include "HeadUpDisplay.h"

Boss::Boss() : CGameObject()
{
	hp = 16;
	score = 3000;

	this->isEnemy = true;
	this->isActive = true;
	isFlyToSimon = false;
	SetState(BOSS_STATE_IDLE);
}

void Boss::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->state != BOSS_STATE_EGG)
	{
		left = x;
		top = y;
		right = x + BOSS_BBOX_WIDTH;
		bottom = y + BOSS_BBOX_HEIGHT;
	}
	else {
		left = x;
		top = y;
		right = x + EGG_BBOX_WIDTH;
		bottom = y + EGG_BBOX_HEIGHT;
	}
}

void Boss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->state == BOSS_STATE_IDLE) {
		waiting = GetTickCount();
		return;
	}

	if (hp < 1) {
		this->SetState(BOSS_STATE_DIE);
		hp = 0;
	}

	HeadUpDisplay::GetInstance()->SetBossHP(hp);

	if (this->state == BOSS_STATE_EGG) {
		vy += BOSS_GRAVITY * dt;
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
			x += min_tx * dx + nx * 0.4f;	// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
				{
					if (ny != 0) {
						dx = dy = 0;
					}
				}
				if (dynamic_cast<Simon*>(e->obj)) // if e->obj is Ground
				{
					if (this->state == BOSS_STATE_EGG) {
						this->enable = false;
						HeadUpDisplay::GetInstance()->AddScore(1000);
					}
				}
				else {
					x += dx;
					y += dy;
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		return;
	}
	/*for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Whip*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			this->LostHp(5);
		}
	}*/

	if (this->state == BOSS_STATE_DIE) {
		if (animation_set->at(BOSS_STATE_DIE)->isOver(300))
		{
			// this->enable = false;
			this->SetState(BOSS_STATE_EGG);
		}
	}

	if (isWaiting)
	{
		if (GetTickCount() - waiting > BOSS_TIME_WAITING)
		{
			vx = vy = 0;
			isWaiting = false;
		}
		else return;
	}

	if (!isFlyToPoint)
	{
		isFlyToPoint = true;

		// Calculate point fly
		if (count == 1)
		{
			isFlyToSimon = true;
			Simon::GetInstance()->GetPosition(point.x, point.y);
		}
		else
		{
			point = GetRandomPoint();
		}

		// Calculate velocity
		CalculateVelocity();
	}
	else
		FlyToPoint(dt);
	
	/*point = GetRandomPoint();
	FlyToPoint(dt);*/
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
	case BOSS_STATE_EGG:
		vx = 0;
		animation_set->at(BOSS_STATE_DIE)->resetAnimation();
		animation_set->at(BOSS_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}

void Boss::GetActiveBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 100;
	top = y;
	right = left + 10;
	bottom = y + 800;
}

D3DXVECTOR2 Boss::GetRandomPoint()
{
	D3DXVECTOR2 randomPoint;

	//float left = this->x - BOSS_BBOX_FLY_WIDTH;
	float left = 1129;
	float top = 80;

	float distance = 0;

	do
	{
		randomPoint.x = left + rand() % (2 * BOSS_BBOX_FLY_WIDTH);
		randomPoint.y = top + rand() % (BOSS_BBOX_FLY_HEIGHT);

		float dx = abs(x - randomPoint.x);
		float dy = abs(y - randomPoint.y);

		distance = sqrt(pow(x - randomPoint.x, 2) + pow(y - randomPoint.y, 2));
	} while (distance < 100.0f);

	return randomPoint;
}

void Boss::CalculateVelocity()
{
	float dx = abs(this->x - point.x);
	float dy = abs(this->y - point.y);

	int nx, ny;

	if (x < point.x) nx = 1;
	else nx = -1;

	if (y < point.y) ny = 1;	
	else ny = -1;

	if (isFlyToSimon == true)
	{
		vx = nx * dx / 750;
		vy = ny * dy / 750;
	}
	else
	{
		vx = nx * dx / 1000;
		vy = ny * dy / 1000;
	}
}

void Boss::FlyToPoint(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;

	if (abs(x - point.x) <= 1.0f)
	{
		isFlyToPoint = false;
		this->SetPosition(point.x, point.y);

		count = (count + 1) % 3;

		if (isFlyToSimon == true)
		{
			isFlyToSimon = false;
		}
		else
		{
			StartWaiting();
		}
	}
}
