#include "Whip.h"
#include "Candle.h"
#include "SmallCandle.h"
#include "BlackKnight.h"
#include "VampireBat.h"
#include "Ghost.h"
#include "Fleaman.h"
#include "Raven.h"
#include "Skeleton.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<Candle*>(obj) || dynamic_cast<SmallCandle*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(CANDLE_STATE_DESTROYED);
				}
			}
		}
		if (dynamic_cast<BlackKnight*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(BLACKKNIGHT_STATE_DIE);
				}
			}
		}
		if (dynamic_cast<VampireBat*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(VAMPIREBAT_STATE_DIE);
				}
			}
		}
		if (dynamic_cast<Ghost*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && obj->GetState() != GHOST_STATE_IDLE && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(GHOST_STATE_DIE);
				}
			}
		}
		if (dynamic_cast<Fleaman*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(FLEAMAN_STATE_DIE);
				}
			}
		}
		if (dynamic_cast<Raven*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(RAVEN_STATE_DIE);
				}
			}
		}
		if (dynamic_cast<Skeleton*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(SKELETON_STATE_DIE);
				}
			}
		}
	}
}

void Whip::Render()
{
	if (this->nx == -1) animation_set->at(state)->Render(x, y);
	else animation_set->at(state + 1)->Render(x, y);

	/*if (this->nx == -1) animation_set->at(state)->Render(x, y);
	else animation_set->at(state + 1)->Render(x, y);*/

	RenderBoundingBox();
}

void Whip::SetWhipPosition(int simonX, int simonY, bool isStand)
{
	this->x = simonX - 90.0f;
	this->y = simonY;
	if (!isStand)
	{
		this->y = simonY + 15.0f;
	}
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->nx == -1)
	{
		left = x + 50;
		top = y + 15;
		bottom = top + WHIP_BBOX_HEIGHT;
		if (this->state < STATE_CHAIN_LONG_RIGHT) right = left + WHIP_BBOX_WIDTH
		else right = left + LONG_CHAIN_BBOX_WIDTH;
	}
	else {
		top = y + 15;
		bottom = top + WHIP_BBOX_HEIGHT;
		right = x + 190;
		if (this->state < STATE_CHAIN_LONG_RIGHT) left = right - WHIP_BBOX_WIDTH
		else left = right - LONG_CHAIN_BBOX_WIDTH;
	}
}

bool Whip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	return CGameObject::AABB(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
}

void Whip::StartHit()
{
	isRender = GetTickCount();
}