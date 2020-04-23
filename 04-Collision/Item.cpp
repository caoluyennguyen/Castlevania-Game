#include "Item.h"
#include "Candle.h"

Item::Item(int type) : CGameObject()
{
	this->typeOfItem = type;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (typeOfItem)
	{
	case 0:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 18;
		break;
	case 2:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
	case 3:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
	case 4:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
	default:
		break;
	}
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
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
	animation_set->at(typeOfItem)->Render(x, y);

	RenderBoundingBox();
}

bool Item::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float item_left,
		item_top,
		item_right,
		item_bottom;

	GetBoundingBox(item_left, item_top, item_right, item_bottom);

	return CGameObject::AABB(item_left, item_top, item_right, item_bottom, obj_left, obj_top, obj_right, obj_bottom);
}