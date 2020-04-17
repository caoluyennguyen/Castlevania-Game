#include "Weapon.h"

void Weapon::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	
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
