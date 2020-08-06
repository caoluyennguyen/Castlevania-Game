#include "Item.h"
#include "Candle.h"
#include "SmallCandle.h"
#include "Ground.h"
#include "Simon.h"
#include "BreakableWall.h"

Item::Item(int type) : CGameObject()
{
	//this->typeOfItem = type;
	this->SetState(type);
	this->enable = true;
}

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (this->state)
	{
	case ITEM_DAGGER:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 18;
		break;
	case ITEM_AXE:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 28;
		break;
	case ITEM_HEART:
		left = x;
		top = y;
		right = x + 24;
		bottom = y + 20;
		break;
	case ITEM_CHAIN:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 32;
		break;
	case ITEM_BOOMERANG:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 28;
		break;
	case ITEM_MONEY:
		left = x;
		top = y;
		right = x + 30;
		bottom = y + 30;
		break;
	case ITEM_SMALLHEART:
		left = x;
		top = y;
		right = x + 16;
		bottom = y + 16;
		break;
	case ITEM_TRIPPLESHOT:
		left = x;
		top = y;
		right = x + 28;
		bottom = y + 28;
		break;
	case ITEM_HOLYWATER:
		left = x;
		top = y;
		right = x + 28;
		bottom = y + 28;
		break;
	case ITEM_INVISIBILITYPOTION:
		left = x;
		top = y;
		right = x + 29;
		bottom = y + 36;
		break;
	case ITEM_CROWN:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 32;
		break;
	case ITEM_DOUBLESHOT:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 32;
		break;
	case ITEM_ONEPIECE:
		left = x;
		top = y;
		right = x + 32;
		bottom = y + 32;
		break;
	default:
		break;
	}
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CGameObject::Update(dt, coObjects);

	//if (GetTickCount() - timeDisappear > 1500 && this->enable) this->enable = false;

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
				else {
					this->enable = true;
					StartTimeDisappear();
				}
			}
		}
		else if (dynamic_cast<SmallCandle*>(obj))
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
				else {
					this->enable = true;
					StartTimeDisappear();
				}
			}
		}
		else if (dynamic_cast<BreakableWall*>(obj))
		{
			BreakableWall* e = dynamic_cast<BreakableWall*> (obj);

			float left, top, right, bottom;

			e->GetBoundingBox(left, top, right, bottom);

			if (CheckCollision(left, top, right, bottom) == true)
			{
				if (e->GetState() == NORMAL)
				{
					this->enable = false;
				}
				else {
					this->enable = true;
					StartTimeDisappear();
				}
			}
		}
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (this->isEnable())
	{
		if (this->state == ITEM_SMALLHEART && isShake) {
			vx += ITEM_SMALLHEART_VX * shake;
			if (vx > ITEM_SMALLHEART_VX_MAX) shake = -1;
			else if (vx < ITEM_SMALLHEART_VX_MIN) shake = 1;
		}
		vy += gravity * dt;

		if (coEvents.size() == 0 && vx != 0)
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
					if (ny < 0) {
						isShake = false;
						vx = 0;
					}
				}
				else if (dynamic_cast<Simon*>(e->obj))
				{
					//this->enable = false;
					vy = 0;
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
	alpha = 255;
	if (GetTickCount() - timeDisappear > 1000) alpha = 128;

	animation_set->at(this->state)->Render(x, y, alpha);

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
		gravity = 0.0001f;
		//animation_set->at(CANDLE_STATE_DESTROYED)->resetAnimation();
		//animation_set->at(CANDLE_STATE_DESTROYED)->setStartFrameTime(GetTickCount());
	default:
		break;
	}
}