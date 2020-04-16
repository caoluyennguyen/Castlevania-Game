#include "Whip.h"
#include "Candle.h"

void Whip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}

void Whip::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}

void Whip::SetWhipPosition(int simonX, int simonY, bool isStand)
{
	x = simonX - 90.0f;
	y = simonY;
	if (!isStand)
	{
		this->y = simonY + 15.0f;
	}
}

void Whip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x + 50;
	top = y + 15;
	right = left + WHIP_BBOX_WIDTH;
	bottom = top + WHIP_BBOX_HEIGHT;
}