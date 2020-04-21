#include "Weapon.h"
#include "Candle.h"

Weapon::Weapon() : CGameObject()
{
	this->enable = false;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += dx;

	CGameObject::Update(dt, coObjects);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Candle*>(obj))
		{
			Candle* e = dynamic_cast<Candle*> (obj);

			float left, top, right, bottom;

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
		animation_set->at(0)->Render(x, y);
	}
	else animation_set->at(1)->Render(x, y);
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
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}