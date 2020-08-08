#include "Skeleton.h"
#include "Ground.h"
#include "Simon.h"

Skeleton::Skeleton(int nx) : CGameObject()
{
	this->nx = nx;
	this->isActive = true;
	this->isEnemy = true;
	this->isWaiting = false;
	this->weapon = new Weapon();
	this->weapon->nx = nx;
	this->weapon->SetState(BONE);
	SetState(SKELETON_STATE_WALK);
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

		// handle bone
		if (weapon->isEnable() && isAbleToThrowBone) {
			weapon->Update(dt, coObjects);
		}
		
		// Skeleton moving
		if (GetTickCount() - waiting > SKELETON_THROW_BONE_DELAY)
		{
			if (isWaiting)
			{
				SetState(SKELETON_STATE_IDLE);
			}
			else {
				SetState(SKELETON_STATE_WALK);
			}
		}
		if (this->state == SKELETON_STATE_WALK)
		{
			vx += move * 0.02f;
			if (vx > 0.2f) move = -1;
			else if (vx < -0.2f)  move = 1;
		}
		
		vy += 0.01f;
		x += dx;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
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
					//this->SetState(SKELETON_STATE_WALK);
					if (e->ny != 0) vy = 0;
				}
				/*else {
					y += dy;
				}*/
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
	if (weapon->isEnable() && isAbleToThrowBone){
		weapon->Render();
	}

	//RenderBoundingBox();
	//RenderActiveBoundingBox();
}

void Skeleton::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SKELETON_STATE_IDLE:
		this->isWaiting = false;
		StartWaiting();

		vx = vy = 0;

		weapon->SetPosition(this->x, this->y);
		weapon->vy = -0.4f;
		weapon->vx = nx * 0.1f;
		weapon->enable = true;
		break;
	case SKELETON_STATE_WALK:
		this->isWaiting = true;
		this->isAbleToThrowBone = false;
		StartWaiting();

		weapon->vx = this->weapon->vy = 0;
		weapon->enable = false;
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
	if (nx == -1)
	{
		left = x - 100;
		top = y - 100;
		right = left + 10;
		bottom = top + 600;
	}
	else {
		left = x + 100;
		top = y - 100;
		right = left + 10;
		bottom = top + 600;
	}
}