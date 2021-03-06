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
#include "VampireBat.h"
#include "SmallCandle.h"
#include "Elevator.h"
#include "Ghost.h"
#include "Fleaman.h"
#include "Raven.h"
#include "Skeleton.h"
#include "Zombie.h"
#include "Boss.h"
#include "Utils.h"
#include "HeadUpDisplay.h"
#include "BreakableWall.h"

Simon* Simon::__instance = NULL;

Simon* Simon::GetInstance()
{
	if (__instance == NULL) __instance = new Simon();
	return __instance;
}

void Simon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//DebugOut(L"dt: %d\n", dt);
	//if (dt > 16) dt = 16;

	if (playerHP < 1) {
		if (nx > 1) {
			this->SetState(SIMON_STATE_DIE_RIGHT);
		}
		else {
			this->SetState(SIMON_STATE_DIE_LEFT);
		}

		untouchable = 0;
	}
	
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (!this->isStepOnStair) vy += SIMON_GRAVITY * dt;
	else {
		this->isOnGround = false;
	}

	// Update whip
	if (whip->enable)
	{
		whip->state = this->whip_level;
		whip->Update(dt, coObjects);
		whip->SetWhipPosition(this->x, this->y, this->isStand);
	}

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
		x += min_tx * dx + nx * 0.4f;	// nx*0.4f : need to push out a bit to avoid overlapping next frame
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
				else if (ny != 0)
				{
					vy = 0;
					isOnGround = true;
					isStepOnStair = false;
				}
				else if (nx != 0)
				{
					vx = 0;
				}
				else
				{
					y += dy;
					isOnGround = false;
				}
			}
			if (dynamic_cast<Candle*>(e->obj) || dynamic_cast<SmallCandle*>(e->obj) || dynamic_cast<Weapon*>(e->obj) ||
				dynamic_cast<UpStair*>(e->obj) || dynamic_cast<DownStair*>(e->obj)) // if e->obj is Ground
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
				/*if (e->nx != 0) x += 0;
				if (e->ny != 0) y += 0;*/
			}
			if (dynamic_cast<Item*>(e->obj)) // if e->obj is Ground
			{
				//Item* item = dynamic_cast<Item*>(e->obj);
				//if (item->isHide) continue;
				switch (e->obj->GetState())
				{
				case ITEM_DAGGER:
					this->weapon = 1;
					break;
				case ITEM_AXE:
					this->weapon = 2;
					break;
				case ITEM_BOOMERANG:
					this->weapon = 3;
					break;
				case ITEM_HOLYWATER:
					this->weapon = 4;
					break;
				case ITEM_CHAIN:
					if (nx == 1)
					{
						SetState(SIMON_STATE_GET_ITEM_LEFT);
					}
					else SetState(SIMON_STATE_GET_ITEM_RIGHT);
					this->whip_level += 2;
					break;
				case ITEM_HEART:
					this->heart += 5;
					break;
				case ITEM_SMALLHEART:
					this->heart += 1;
					break;
				case ITEM_MONEY:
					this->AddScore(1000);
					HeadUpDisplay::GetInstance()->AddScore(1000);
					break;
				case ITEM_TRIPPLESHOT:
					//this->weapon_count = 3;
					break;
				default:
					break;
				}
				
				//HeadUpDisplay::GetInstance()->AddScore(100);
				e->obj->enable = false;
				if (e->nx != 0) x += dx;
				if (e->ny != 0) {
					y -= 5;
				}
			}
			if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
				break;
			}
			if (dynamic_cast<BreakableWall*>(e->obj))
			{
				if (e->obj->GetState() == BREAK) {
					if (e->nx != 0) x += dx;
					if (e->ny != 0) y += dy;
				}
				else {
					if (ny != 0)
					{
						vy = 0;
						isOnGround = true;
						isStepOnStair = false;
					}
				}
				break;
			}
			if (e->obj->isEnemy)
			{
				if (dynamic_cast<Boss*>(e->obj)) {
					Boss* p = dynamic_cast<Boss*>(e->obj);
					if (p->GetState() == BOSS_STATE_EGG) {
						y -= 5;
						p->enable = false;
						HeadUpDisplay::GetInstance()->AddScore(1000);
						return;
					}
				}

				/*if (dynamic_cast<Raven*>(e->obj)) {
					Raven* p = dynamic_cast<Raven*>(e->obj);
					p->SetState(RAVEN_STATE_DIE);
				}*/

				if (this->untouchable == 0 && e->obj->isActive)
				{
					if (e->nx > 0) {
						x += dx;
						this->SetState(SIMON_STATE_INJURED_LEFT);
					}
					else {
						x += dx;
						this->SetState(SIMON_STATE_INJURED_RIGHT);
					}
					if (e->ny != 0) {
						y += dy;
						//vy = 0;
						if (nx > 0) this->SetState(SIMON_STATE_INJURED_LEFT);
						else this->SetState(SIMON_STATE_INJURED_RIGHT);
					}
				}
				else if (!e->obj->isActive) {
					x += dx;
					if (e->ny != 0) y += dy;
				}
			}
			if (dynamic_cast<Elevator*>(e->obj))
			{
				Elevator* p = dynamic_cast<Elevator*>(e->obj);
				if (ny == -1)
				{
					this->vx = p->vx;
					vy = 0;
					isOnGround = true;
					isStandOnElevator = true;
					isStepOnStair = false;
				}
				else if (nx != 0) {
					this->x += dx;
					dy = 0;
				}
				else
				{	
					vy = 0;
					isOnGround = false;
				}
			}
			else isStandOnElevator = false;
		}
	}

	// Handle Simon step on stair
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (obj->isEnemy) {
			if (dynamic_cast<VampireBat*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					VampireBat* p = dynamic_cast<VampireBat*>(obj);
					p->SetAbleToFly();
				}
			}
			if (dynamic_cast<Ghost*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					Ghost* p = dynamic_cast<Ghost*>(obj);
					p->SetState(GHOST_STATE_FLY_LEFT);
					p->isActive = true;
				}
			}
			if (dynamic_cast<Fleaman*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					Fleaman* p = dynamic_cast<Fleaman*>(obj);
					if (p->GetState() == FLEAMAN_STATE_IDLE_RIGHT) p->SetState(FLEAMAN_STATE_JUMP_RIGHT);
				}
			}
			if (dynamic_cast<Raven*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					Raven* p = dynamic_cast<Raven*>(obj);
					p->SetState(RAVEN_STATE_FLY_LEFT);
				}
			}
			if (dynamic_cast<Skeleton*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					Skeleton* p = dynamic_cast<Skeleton*>(obj);
					p->isAbleToThrowBone = true;
				}
			}
			if (dynamic_cast<Zombie*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					Zombie* p = dynamic_cast<Zombie*>(obj);
					//p->SetState(ZOMBIE_STATE_DIE);
					p->isActive = true;
				}
			}
			if (dynamic_cast<Boss*>(obj))
			{
				float left, top, right, bottom;
				obj->GetActiveBoundingBox(left, top, right, bottom);

				if (this->CheckCollision(left, top, right, bottom) && obj->isEnable())
				{
					this->isFightingBoss = true;
					Boss* p = dynamic_cast<Boss*>(obj);
					p->SetState(BOSS_STATE_FLY);
					//p->isActive = true;
				}
			}
		}

		if (dynamic_cast<UpStair*>(obj))
		{
			if (isTouchUpStair) continue;

			if (obj->nx != this->nx); // if simon is back to stair do something
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom))
			{
				isTouchUpStair = true;
				if (this->isOnGround && !this->isMoveToStairUp) {
					this->isAbleToStepDownStair = true;
					this->nxUpStair = obj->nx;
					this->nxDownStair = -obj->nx;
				}
				if (this->nxUpStair == 1) {
					if (!(this->isStepOnStair))
					{
						distanceUp = int(left - this->x);
						if (isOnGround) this->isAbleToMoveToStair = true;
						else this->isAbleToMoveToStair = false;
					}

					if (this->isMoveToStairDown) {
						if (distanceUp != 0) {
							if (abs(distanceUp) > dx) this->x += dx;
							else this->x += distanceUp;
							if (distanceUp > 0) this->SetState(SIMON_STATE_WALKING_RIGHT);
							else this->SetState(SIMON_STATE_WALKING_LEFT);
							auto_start = GetTickCount();
							//return;
						}
						else {
							distanceUp = 0;
							if (GetTickCount() - auto_start < 290)
							{
								this->SetState(SIMON_GO_DOWNSTAIR_RIGHT);
							}
							else {
								this->SetState(SIMON_STAND_ON_DOWNSTAIR_RIGHT);
								this->isMoveToStairDown = false;
								this->isAbleToMoveToStair = false;
							}
						}
						
						break;
					}
					else {
						// Xu ly khi di len thang va chan cham dat
						if (y + SIMON_BBOX_HEIGHT < bottom) {
							this->isAbleToStepUpStair = false;
							this->isStepOnStair = false;
							if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
							else this->SetState(SIMON_STATE_IDLE_LEFT);
						}
					}
				}
				else {
					if (!(this->isStepOnStair))
					{
						distanceUp = int(right - this->x - SIMON_BBOX_WIDTH);
						if (isOnGround) this->isAbleToMoveToStair = true;
						else this->isAbleToMoveToStair = false;
					}

					if (this->isMoveToStairDown) {
						if (distanceUp != 0) {
							if (distanceUp > 0) this->SetState(SIMON_STATE_WALKING_RIGHT);
							else this->SetState(SIMON_STATE_WALKING_LEFT);
							if (abs(distanceUp) > dx) this->x += dx;
							else this->x += distanceUp;
							auto_start = GetTickCount();
							//return;
						}
						else {
							distanceUp = 0;
							if (GetTickCount() - auto_start < 290)
							{
								this->SetState(SIMON_GO_DOWNSTAIR_LEFT);
							}
							else {
								this->SetState(SIMON_STAND_ON_DOWNSTAIR_LEFT);
								this->isMoveToStairDown = false;
								this->isAbleToMoveToStair = false;
							}
						}
						
						break;
					}
					else {
						// Xu ly khi di len thang va chan cham dat
						if (y + SIMON_BBOX_HEIGHT < bottom) {
							this->isAbleToStepUpStair = false;
							this->isStepOnStair = false;
							if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
							else this->SetState(SIMON_STATE_IDLE_LEFT);
						}
					}
				}
				
				break;
			}
			else {
				this->isAbleToMoveToStair = false;
				this->isAbleToStepDownStair = false;
				//this->isTouchUpStair = false;
			}
		}
		if (dynamic_cast<DownStair*>(obj))
		{
			if (isTouchDownStair) continue;

			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom))
			{
				isTouchDownStair = true;

				if (this->isOnGround && !this->isMoveToStairDown) {
					this->nxDownStair = obj->nx;
					this->nxUpStair = -obj->nx;
					this->isAbleToStepUpStair = true;
				}
				if (this->nxDownStair == 1)
				{
					if (!(this->isStepOnStair))
					{
						distanceDown = int(left - this->x);
						if (isOnGround) this->isAbleToMoveToStair = true;
						else this->isAbleToMoveToStair = false;
					}

					if (this->isMoveToStairUp) {
						if (distanceDown != 0) {
							if (abs(distanceDown) > dx) this->x += dx;
							else this->x += distanceDown;
							if (distanceDown > 0) this->SetState(SIMON_STATE_WALKING_RIGHT);
							else this->SetState(SIMON_STATE_WALKING_LEFT);
							auto_start = GetTickCount(); // problem when distance = 0 simon do not move correctly
							//return;
						}
						else {
							distanceDown = 0;
							if (GetTickCount() - auto_start < 200)
							{
								this->SetState(SIMON_GO_UPSTAIR_RIGHT);
							}
							else {
								this->SetState(SIMON_STAND_ON_UPSTAIR_RIGHT);
								this->isMoveToStairUp = false;
								this->isAbleToMoveToStair = false;
							}
						}
						
						//break;
					}
					else {
						/*this->nxDownStair = obj->nx;
						this->nxUpStair = -obj->nx;*/

						// Xu ly khi di xuong thang va chan cham dat
						if (y + SIMON_BBOX_HEIGHT > bottom && this->isStepOnStair && !(this->isMoveToStairUp)) {
							this->isStepOnStair = false;
							if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
							else this->SetState(SIMON_STATE_IDLE_LEFT);
						}
					}
				}
				else {
					if (!(this->isStepOnStair))
					{
						distanceDown = int(right - this->x - SIMON_BBOX_WIDTH);
						if (isStand) this->isAbleToMoveToStair = true;
						else this->isAbleToMoveToStair = false;
					}

					if (this->isMoveToStairUp) {
						if (distanceDown != 0) {
							if (distanceDown > 0) this->SetState(SIMON_STATE_WALKING_RIGHT);
							else this->SetState(SIMON_STATE_WALKING_LEFT);
							if (abs(distanceDown) > dx) this->x += dx;
							else this->x += distanceDown;
							auto_start = GetTickCount();
							//return;
						}
						else {
							distanceDown = 0;
							if (GetTickCount() - auto_start < 200)
							{
								this->SetState(SIMON_GO_UPSTAIR_LEFT);
							}
							else {
								this->SetState(SIMON_STAND_ON_UPSTAIR_LEFT);
								this->isMoveToStairUp = false;
								this->isAbleToMoveToStair = false;
							}
						}

						break;
					}
					else {
						this->nxDownStair = obj->nx;
						this->nxUpStair = -obj->nx;

						// Xu ly khi di xuong thang va chan cham dat
						if (y + SIMON_BBOX_HEIGHT > bottom && this->isStepOnStair && !(this->isMoveToStairUp)) {
							this->isStepOnStair = false;
							if (this->nx == 1) this->SetState(SIMON_STATE_IDLE_RIGHT);
							else this->SetState(SIMON_STATE_IDLE_LEFT);
						}
					}
				}

				break;
			}
			else {
				this->isAbleToMoveToStair = false;
				this->isAbleToStepUpStair = false;
				//this->isTouchDownStair = false;
			}
		}

	}

	isTouchUpStair = isTouchDownStair = false;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Simon::Render()
{
	alpha = 255;
	if (untouchable) alpha = rand() % 255;
	animation_set->at(state)->Render(x, y, alpha);
	//RenderBoundingBox();

	// Render whip
	if (this->GetState() == SIMON_STATE_HIT_SIT || this->GetState() == SIMON_STATE_HIT_STAND
		|| this->GetState() == SIMON_STATE_HIT_SIT_RIGHT || this->GetState() == SIMON_STATE_HIT_STAND_RIGHT
		|| this->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT || this->GetState() == SIMON_HIT_DOWNSTAIR_LEFT
		|| this->GetState() == SIMON_HIT_UPSTAIR_RIGHT || this->GetState() == SIMON_HIT_UPSTAIR_LEFT)
	{
		if (this->isThrowWeapon == false)
		{
			whip->nx = this->nx;
			whip->Render();
		}

	}
	else whip->enable = false;
}

void Simon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_IDLE_LEFT:
		if (!isStandOnElevator) vx = 0;
		nx = -1;
		isStepOnStair = false;
		//isStand = true;
		break;
	case SIMON_STATE_IDLE_RIGHT:
		if (!isStandOnElevator) vx = 0;
		nx = 1;
		isStepOnStair = false;
		//isStand = true;
		break;
	case SIMON_STATE_WALKING_RIGHT:
		if (!this->isMoveToStairUp && !this->isMoveToStairDown) vx = SIMON_WALKING_SPEED;
		else vx = 0.05f;
		nx = 1;
		break;
	case SIMON_STATE_WALKING_LEFT:
		if (!this->isMoveToStairUp && !this->isMoveToStairDown) vx = -SIMON_WALKING_SPEED;
		else vx = -0.05f;
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
		//vy = -SIMON_DIE_DEFLECT_SPEED;
		//StartUntouchable();
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
		isStepOnStair = true;
		isStandUpStair = true;
		isStandDownStair = false;
		animation_set->at(SIMON_GO_UPSTAIR_RIGHT)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_GO_UPSTAIR_LEFT:
		vx = -SIMON_WALKING_SPEED;
		vy = -SIMON_WALKING_SPEED;
		nx = -1;
		isStepOnStair = true;
		isStandUpStair = true;
		isStandDownStair = false;
		animation_set->at(SIMON_GO_UPSTAIR_LEFT)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_GO_DOWNSTAIR_RIGHT:
		vx = SIMON_WALKING_SPEED;
		vy = SIMON_WALKING_SPEED;
		nx = 1;
		isStepOnStair = true;
		isStandUpStair = false;
		isStandDownStair = true;
		animation_set->at(SIMON_GO_DOWNSTAIR_RIGHT)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_GO_DOWNSTAIR_LEFT:
		vx = -SIMON_WALKING_SPEED;
		vy = SIMON_WALKING_SPEED;
		nx = -1;
		isStepOnStair = true;
		isStandUpStair = false;
		isStandDownStair = true;
		animation_set->at(SIMON_GO_DOWNSTAIR_LEFT)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STAND_ON_UPSTAIR_RIGHT:
		vx = vy = 0;
		nx = 1;
		isStepOnStair = true;
		isStandUpStair = true;
		isStandDownStair = false;
		animation_set->at(SIMON_GO_UPSTAIR_RIGHT)->resetAnimation();
		break;
	case SIMON_STAND_ON_UPSTAIR_LEFT:
		vx = vy = 0;
		nx = -1;
		isStepOnStair = true;
		isStandUpStair = true;
		isStandDownStair = false;
		animation_set->at(SIMON_GO_UPSTAIR_LEFT)->resetAnimation();
		break;
	case SIMON_STAND_ON_DOWNSTAIR_RIGHT:
		vx = vy = 0;
		nx = 1;
		isStepOnStair = true;
		isStandUpStair = false;
		isStandDownStair = true;
		animation_set->at(SIMON_GO_DOWNSTAIR_RIGHT)->resetAnimation();
		break;
	case SIMON_STAND_ON_DOWNSTAIR_LEFT:
		vx = vy = 0;
		nx = -1;
		isStepOnStair = true;
		isStandUpStair = false;
		isStandDownStair = true;
		animation_set->at(SIMON_GO_DOWNSTAIR_LEFT)->resetAnimation();
		break;
	case SIMON_STATE_INJURED_RIGHT:
		this->SubtractPlayerHP(1);
		vy = -0.2f;
		vx = -0.2f;
		StartUntouchable();
		animation_set->at(SIMON_STATE_INJURED_RIGHT)->setStartFrameTime(GetTickCount());
		isStepOnStair = false;
		break;
	case SIMON_STATE_INJURED_LEFT:
		this->SubtractPlayerHP(1);
		vy = -0.2f;
		vx = 0.2f;
		StartUntouchable();
		animation_set->at(SIMON_STATE_INJURED_LEFT)->setStartFrameTime(GetTickCount());
		isStepOnStair = false;
		break;
	case SIMON_STATE_DIE_LEFT:
		vx = 0;
		animation_set->at(SIMON_STATE_INJURED_LEFT)->setStartFrameTime(GetTickCount());
		break;
	case SIMON_STATE_DIE_RIGHT:
		vx = 0;
		animation_set->at(SIMON_STATE_INJURED_LEFT)->setStartFrameTime(GetTickCount());
		break;
	}
}

void Simon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 12;
	top = y + 4;

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

void Simon::Reset()
{
	this->SetState(SIMON_STATE_IDLE_RIGHT);
	this->SetSpeed(0, 0); 
	this->SetPosition(900.0f, 0.0f);
	playerHP = 16;
	heart = 16;
	score = 0;
	weapon = -1;
	weapon_count = 1;
	isFightingBoss = false;

	untouchable = 0;
	whip_level = 0;
}