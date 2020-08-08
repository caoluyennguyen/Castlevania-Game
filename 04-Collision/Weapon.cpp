#include "Weapon.h"
#include "Candle.h"
#include "Utils.h"
#include "Ground.h"
#include "Simon.h"
#include "SmallCandle.h"
#include "BlackKnight.h"
#include "Boss.h"
#include "Fleaman.h"
#include "Ghost.h"
#include "Raven.h"
#include "Skeleton.h"
#include "VampireBat.h"
#include "Zombie.h"

Weapon::Weapon() : CGameObject()
{
	this->enable = false;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	if (state == AXE || state == HOLYWATER || state == BONE) vy += WEAPON_GRAVITY * dt;

	if (GetTickCount() - renderHolywater > 500 && this->state == FIRE)
	{
		this->enable = false;
		this->SetState(HOLYWATER);
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		if (GetTickCount() - boomerangBack > 800 && this->state == BOOMERANG)
		{
			nx = nxFlyBack;
			vx = vxFlyBack;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
			{
				if (this->state == HOLYWATER)
				{
					this->SetState(FIRE);
				}
			}
			else if (dynamic_cast<Candle*>(e->obj) || dynamic_cast<SmallCandle*>(e->obj)) // if e->obj is Candle
			{
				if (this->isEnable() && this->state != BONE)
				{
					e->obj->SetState(CANDLE_STATE_DESTROYED);
					this->enable = false;
					Simon::GetInstance()->AddNumOfWeapon();
				}
			}
			else if (dynamic_cast<BlackKnight*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(BLACKKNIGHT_STATE_DIE);
				}
			}
			else if (dynamic_cast<VampireBat*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(VAMPIREBAT_STATE_DIE);
				}
			}
			else if (dynamic_cast<Ghost*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(GHOST_STATE_DIE);
				}
			}
			else if (dynamic_cast<Fleaman*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(FLEAMAN_STATE_DIE);
				}
			}
			else if (dynamic_cast<Raven*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(RAVEN_STATE_DIE);
				}
			}
			else if (dynamic_cast<Skeleton*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(SKELETON_STATE_DIE);
				}
			}
			else if (dynamic_cast<Zombie*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				if (this->enable)
				{
					e->obj->SetState(ZOMBIE_STATE_DIE);
				}
			}
			else if (dynamic_cast<Boss*>(e->obj))
			{
				float left, top, right, bottom;
				e->obj->GetBoundingBox(left, top, right, bottom);

				Boss* boss = dynamic_cast<Boss*>(e->obj);
				boss->LostHp(2);
				if (boss->GetHp() < 1) {
					boss->SetState(BOSS_STATE_DIE);
					boss->SetHp(0);
				}				//boss->SetHp(0);
				//boss->SetState(BOSS_STATE_DIE);
			}
			else if (dynamic_cast<Simon*>(e->obj)) {
				if (this->state == BONE) {
					if (e->nx < 0) {
						e->obj->SetState(SIMON_STATE_INJURED_LEFT);
					}
					else {
						e->obj->SetState(SIMON_STATE_INJURED_RIGHT);
					}
					if (e->ny != 0) {
						//vy = 0;
						if (e->obj->nx > 0) e->obj->SetState(SIMON_STATE_INJURED_LEFT);
						else e->obj->SetState(SIMON_STATE_INJURED_RIGHT);
					}
					this->enable = false;
				}
			}
			else
			{
				if (GetTickCount() - boomerangBack > 800 && this->state == BOOMERANG)
				{
					if (dynamic_cast<Simon*>(e->obj)) {
						this->enable = false;
						Simon::GetInstance()->AddNumOfWeapon();
					}
					nx = nxFlyBack;
					vx = vxFlyBack;
				}
			}

			if (e->obj->isEnemy) {
				this->enable = false;
				Simon::GetInstance()->AddNumOfWeapon();
			}
		}
	}

	x += dx;
	y += dy;

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Weapon::Render()
{
	if (this->state == DAGGER_LEFT || this->state == DAGGER_RIGHT)
	{
		if (this->vx > 0) animation_set->at(DAGGER_RIGHT)->Render(x, y);
		else animation_set->at(DAGGER_LEFT)->Render(x, y);
	}
	else if (this->state == HOLYWATER)
	{
		this->StartRenderHolywater();
		animation_set->at(state)->Render(x, y);
	}
	else if (this->state == BOOMERANG)
	{
		animation_set->at(state)->Render(x, y);
	}
	else animation_set->at(state)->Render(x, y);

	//();
}	

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 32;
	bottom = top + 18;
}

void Weapon::SetState(int state)
{
	this->state = state;

	switch (state)
	{
	case DAGGER_LEFT:
		vx = -0.2f;
		vy = 0;
		break;
	case DAGGER_RIGHT:
		vx = 0.2f;
		vy = 0;
		break;
	case AXE:
		vx = 0.1f;
		break;
	case BOOMERANG:
		vy = 0;
		break;
	case HOLYWATER:
		vx = 0.2f;
		vy = 0.1f;
		animation_set->at(state)->resetAnimation();
		break;
	case FIRE:
		vx = vy = 0;
		break;
	case BONE:
		break;
	default:
		break;
	}
}

void Weapon::SetWeaponPosition(int simonX, int simonY, bool isStand)
{
	this->x = simonX + nx * 30.0f;
	this->y = simonY + 15.0f;
	if (!isStand)
	{
		this->y = simonY + 30.0f;
	}
}

bool Weapon::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float weapon_left,
		weapon_top,
		weapon_right,
		weapon_bottom;

	GetBoundingBox(weapon_left, weapon_top, weapon_right, weapon_bottom);

	return CGameObject::AABB(weapon_left, weapon_top, weapon_right, weapon_bottom, obj_left, obj_top, obj_right, obj_bottom);
}