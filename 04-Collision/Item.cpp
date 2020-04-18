#include "Item.h"
#include "Candle.h"

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 10;
	bottom = y + 10;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//y += dt;
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
				if (e->enable)
				{
					this->enable = false;
				}
				else this->enable = true;
			}
		}
	}
}

void Item::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

bool Item::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}