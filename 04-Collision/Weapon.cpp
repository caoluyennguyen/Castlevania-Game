#include "Weapon.h"

Weapon::Weapon() : CGameObject()
{
	this->enable = false;
}

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
	x += dx;
}

void Weapon::Render()
{
	animation_set->at(0)->Render(x, y);
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
