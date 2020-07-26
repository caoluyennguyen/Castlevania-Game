#include "Item.h"
#include "Candle.h"
#include "SmallCandle.h"
#include "Ground.h"
#include "Simon.h"

Item::Item(int type) : CGameObject()
{
	//this->typeOfItem = type;
	this->state = type;
	this->enable = true;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->state)
	{
	case 0:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 18;
		break;
	case 1:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
	case 2:
		left = x;
		top = y;
		right = x + 24;
		bottom = y + 20;
	case 3:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 32;
	case 4:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 18;
		break;
	case 5:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
	case 6:
		left = x;
		top = y;
		right = x + 24;
		bottom = y + 20;
	default:
		break;
	}
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CGameObject::Update(dt, coObjects);
	vy += ITEM_GRAVITY * dt;

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
		if (dynamic_cast<SmallCandle*>(obj))
		{
			SmallCandle* e = dynamic_cast<SmallCandle*> (obj);

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
		/*if (dynamic_cast<Ground*>(obj))
		{
			Ground* e = dynamic_cast<Ground*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				dx = dy = 0;
			}
			else {
				x += dx;
				y += dy;
			}
		}*/
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (this->isEnable())
	{
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
			x += min_tx * dx + nx * 0.4f;	// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<Ground*>(e->obj)) // if e->obj is Ground
				{
					if (ny != 0) vx = vy = 0;
				}
				else if (dynamic_cast<Simon*>(e->obj))
				{
					this->enable = false;
				}
				else {
					x += dx;
					y += dy;
				}
			}
		}
	}
}

void Item::Render()
{
	animation_set->at(this->state)->Render(x, y);

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

void Item::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ITEM_SMALLHEART:
		//animation_set->at(CANDLE_STATE_DESTROYED)->resetAnimation();
		//animation_set->at(CANDLE_STATE_DESTROYED)->setStartFrameTime(GetTickCount());
	default:
		break;
	}
}