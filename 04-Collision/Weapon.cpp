#include "Weapon.h"
#include "Candle.h"
#include "Utils.h"

#define DAGGER_LEFT 0
#define DAGGER_RIGHT 1

Weapon::Weapon() : CGameObject()
{
	this->enable = true;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += dx;

	CGameObject::Update(dt, coObjects);
	float left, top, right, bottom;

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Candle*>(obj))
		{
			Candle* e = dynamic_cast<Candle*> (obj);

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					e->SetState(CANDLE_STATE_DESTROYED);
					this->enable = false;
				}
			}
		}
	}
}

void Weapon::Render()
{
	if (nx == -1)
	{
		animation_set->at(4)->Render(x, y);
	}
	else animation_set->at(3)->Render(x, y);
	RenderBoundingBox();
}	

void Weapon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + 20;
	bottom = top + 10;
}

void Weapon::SetState(int state)
{
	this->state = state;

	switch (state)
	{
	case 0:
		vx = nx * 0.3f;
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

	return CGameObject::AABB(weapon_left, weapon_left, weapon_right, weapon_bottom, obj_left, obj_top, obj_right, obj_bottom);
}