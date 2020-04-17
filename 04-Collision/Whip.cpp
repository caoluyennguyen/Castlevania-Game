#include "Whip.h"
#include "Candle.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Whip::Render()
{
	if (this->nx == -1) animation_set->at(0)->Render(x, y);
	else animation_set->at(1)->Render(x, y);

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
		right = left + WHIP_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else {
		top = y + 15;
		bottom = top + WHIP_BBOX_HEIGHT;
		right = x + 190;
		left = right - WHIP_BBOX_WIDTH;
	}
}