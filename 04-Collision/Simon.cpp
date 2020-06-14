#include "Simon.h"
#include <algorithm>
#include "debug.h"

#include "Game.h"
#include "Textures.h"

#include "Portal.h"
#include "Ground.h"
#include "Candle.h"
#include "Item.h"
#include "Weapon.h"
#include "UpStair.h"
#include "DownStair.h"

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!this->isStepOnStair) vy += SIMON_GRAVITY * dt;
	else this->isOnGround = false;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != SIMON_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SIMON_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<UpStair*>(obj))
		{
			if (obj->nx != this->nx); // if simon is back to stair do something
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom))
			{
				if (!this->isOnGround) this->isAbleToStepUpStair = true;
				else this->isAbleToStepUpStair = false;
				this->isAbleToStepDownStair = true;
				this->nxUpStair = obj->nx; // get orientation of moving on stair
				this->nxDownStair = -obj->nx; // get orientation of moving on stair

				if (y + SIMON_BBOX_HEIGHT < bottom) {
					this->isAbleToStepUpStair = false;
					this->isStepOnStair = false;
					if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
					else this->SetState(SIMON_STATE_IDLE_LEFT);
				}
			}
			/*else {
				this->isAbleToStepDownStair = false;
			}*/
		}
		if (dynamic_cast<DownStair*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom))
			{
				if (this->x < left) this->isAbleToMoveToStair = true;
				else this->isAbleToMoveToStair = false;

				if (this->isAutoMoveToStair) {
					if (this->x < left) this->SetState(SIMON_STATE_WALKING_RIGHT);
					else {
						this->isAutoMoveToStair = false;
						if (!this->isOnGround) this->isAbleToStepDownStair = true;
						this->isAbleToStepUpStair = true;
						/*this->nxDownStair = obj->nx;
						this->nxUpStair = -obj->nx; */
					}
				}
				else {
					/*this->isStepOnStair = true;
					if (!this->isOnGround) this->isAbleToStepDownStair = true;
					this->isAbleToStepUpStair = true;*/
					this->nxDownStair = obj->nx;
					this->nxUpStair = -obj->nx;

					// Xu ly khi di xuong thang va chan cham dat
					if (y + SIMON_BBOX_HEIGHT > bottom && this->isStepOnStair) {
						this->isAbleToStepDownStair = false;
						this->isStepOnStair = false;
						if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
						else this->SetState(SIMON_STATE_IDLE_LEFT);
					}
				}
				//this->nxDownStair = obj->nx;
				//this->nxUpStair = -obj->nx;
				//// Xu ly khi di xuong thang va chan cham dat
				//if (y + SIMON_BBOX_HEIGHT > bottom && this->isStepOnStair) {
				//	this->isAbleToStepDownStair = false;
				//	this->isStepOnStair = false;
				//	if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
				//	else this->SetState(SIMON_STATE_IDLE_LEFT);
				//}
				break;
			}
			/*else {
				this->isAbleToStepUpStair = false;
			}*/
		}
	}

	// No collision occured, proceed normally
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

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			
			if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
			{
				if (this->isStepOnStair)
				{
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
				/*else if (ny != 0)
				{
					if (ny == -1)
					{
						vy = 0;
						isOnGround = true;
					}
					else
					{
						y += dy;
						isOnGround = false;
					}
				}*/
				else if (ny == -1)
				{
					vy = 0;
					isOnGround = true;
					isStepOnStair = false;
				}
				else
				{
					y += dy;
					isOnGround = false;
				}
			}
			if (dynamic_cast<Candle*>(e->obj) || dynamic_cast<Weapon*>(e->obj)) // if e->obj is Ground
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
				/*if (e->nx != 0) x += 0;
				if (e->ny != 0) y += 0;*/
			}
			if (dynamic_cast<Item*>(e->obj)) // if e->obj is Ground
			{
				//StartUntouchable();
				if (e->obj->GetState() == 3)
				{
					if (nx == 1)
					{
						SetState(SIMON_STATE_GET_ITEM_LEFT);
					}
					else SetState(SIMON_STATE_GET_ITEM_RIGHT);
				}
				
				e->obj->enable = false;
			}
			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			if (dynamic_cast<UpStair*>(e->obj) || dynamic_cast<DownStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	/*int ani;
	if (state == SIMON_STATE_DIE)
		ani = SIMON_ANI_DIE;
	else {
		if (vx == 0)
		{
			if (nx > 0) ani = SIMON_ANI_IDLE_RIGHT;
			else ani = SIMON_ANI_IDLE_LEFT;
		}
		else if (vx > 0)
			ani = SIMON_ANI_WALKING_RIGHT;
		else ani = SIMON_ANI_WALKING_LEFT;
	}*/
	/*if ((this->GetState() == SIMON_STATE_GET_ITEM_RIGHT || this->GetState() == SIMON_STATE_GET_ITEM_LEFT)
		&& this->animation_set->at(this->state)->isOver(1000))
	{
		return;
	}*/
	int alpha = 255;
	/*if (untouchable) {
		if (nx == 1)
		{
			SetState(SIMON_STATE_GET_ITEM_RIGHT);
		}
		else SetState(SIMON_STATE_GET_ITEM_LEFT);
	}*/
	animation_set->at(state)->Render(x, y, alpha);

	RenderBoundingBox();
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_IDLE_LEFT:
		vx = 0;
		nx = -1;
		//isStand = true;
		break;
	case SIMON_STATE_IDLE_RIGHT:
		vx = 0;
		nx = 1;
		//isStand = true;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		vx = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		vx = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STATE_SIT:
		vx = vy = 0;
		//isStand = false;
		break;
	case SIMON_STATE_JUMP:
		vy = -SIMON_JUMP_SPEED_Y;
		isOnGround = false;
		animation_set->at(state)->setStartFrameTime(GetTickCount());
	case SIMON_STATE_DIE:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HIT_STAND:
		isStand = true;
		if (this->isOnGround)
		{
			vx = 0;
		}
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_HIT_SIT:
		isStand = false;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_SIT_RIGHT:
		vx = vy = 0;
		//isStand = false;
		break;
	case SIMON_STATE_JUMP_RIGHT:
		vy = -SIMON_JUMP_SPEED_Y;
		isOnGround = false;
		animation_set->at(state)->setStartFrameTime(GetTickCount());
	case SIMON_STATE_DIE_RIGHT:
		vy = -SIMON_DIE_DEFLECT_SPEED;
		break;
	case SIMON_STATE_HIT_STAND_RIGHT:
		isStand = true;
		if (this->isOnGround)
		{
			vx = 0;
		}
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_HIT_SIT_RIGHT:
		isStand = false;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		isStand = false;
		break;
	case SIMON_STATE_GET_ITEM_RIGHT:
		vx = 0;
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_GET_ITEM_LEFT:
		vx = 0;
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_HIT_DOWNSTAIR_RIGHT:
		vx = vy = 0;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_HIT_DOWNSTAIR_LEFT:
		vx = vy = 0;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_HIT_UPSTAIR_RIGHT:
		vx = vy = 0;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_HIT_UPSTAIR_LEFT:
		vx = vy = 0;
		animation_set->at(state)->resetAnimation();
		animation_set->at(state)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_GO_UPSTAIR_RIGHT:
		vx = SIMON_WALKING_SPEED;
		vy = -SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_GO_UPSTAIR_LEFT:
		vx = -SIMON_WALKING_SPEED;
		vy = -SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_GO_DOWNSTAIR_RIGHT:
		vx = SIMON_WALKING_SPEED;
		vy = SIMON_WALKING_SPEED;
		nx = 1;
		break;
	case SIMON_GO_DOWNSTAIR_LEFT:
		vx = -SIMON_WALKING_SPEED;
		vy = SIMON_WALKING_SPEED;
		nx = -1;
		break;
	case SIMON_STAND_ON_UPSTAIR_RIGHT:
		vx = vy = 0;
		nx = 1;
		//isStand = true;
		break;
	case SIMON_STAND_ON_UPSTAIR_LEFT:
		vx = vy = 0;
		nx = -1;
		//isStand = true;
		break;
	case SIMON_STAND_ON_DOWNSTAIR_RIGHT:
		vx = vy = 0;
		nx = 1;
		//isStand = true;
		break;
	case SIMON_STAND_ON_DOWNSTAIR_LEFT:
		vx = vy = 0;
		nx = -1;
		//isStand = true;
		break;
	}
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 12;
	top = y;

	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

void Simon::StartUntouchable()
{
	untouchable = 1;
	untouchable_start = GetTickCount();
}

bool Simon::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float simon_left,
		simon_top,
		simon_right,
		simon_bottom;

	GetBoundingBox(simon_left, simon_top, simon_right, simon_bottom);

	return CGameObject::AABB(simon_left, simon_top, simon_right, simon_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Simon::AutoMoveToStair(LPGAMEOBJECT obj)
{

}