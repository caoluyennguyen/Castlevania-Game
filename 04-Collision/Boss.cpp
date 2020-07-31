#include "Boss.h"
#include "Whip.h"
#include "HeadUpDisplay.h"

Boss::Boss(Simon *simon) : CGameObject()
{
	hp = 16;
	score = 3000;
	isWaiting = GetTickCount();
	this->simon = simon;

	this->isEnemy = true;
	this->isActive = true;
	isFlyToSimon = false;
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
	if (this->state == BOSS_STATE_IDLE) return;
	HeadUpDisplay::GetInstance()->SetBossHP(hp);

	CGameObject::Update(dt);
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

	if (hp == 0) this->SetState(BOSS_STATE_DIE);

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

	if (isWaiting)
	{
		if (GetTickCount() - waiting > BOSS_TIME_WAITING)
		{
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
			this->simon->GetPosition(point.x, point.y);
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

D3DXVECTOR2 Boss::GetRandomPoint()
{
	D3DXVECTOR2 randomPoint;

	//float left = this->x - BOSS_BBOX_FLY_WIDTH;
	float left = 1129;
	float top = 32;

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
	float dx = abs(x - point.x);
	float dy = abs(y - point.y);

	int nx, ny;

	if (x < point.x) nx = 1;
	else nx = -1;

	if (y < point.y) ny = 1;
	else ny = -1;

	if (isFlyToSimon == true)
	{
		vx = nx * 0.05f;
		vy = ny * 0.05f;
	}
	else
	{
		vx = nx * 0.02f;
		vy = ny * 0.02f;
	}
}

void Boss::FlyToPoint(DWORD dt)
{
	x += dx;
	y += dy;

	if (abs(x - point.x) <= 1.0f)
	{
		isFlyToPoint = false;

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
