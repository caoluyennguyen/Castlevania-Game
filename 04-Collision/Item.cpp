#include "Item.h"

void Item::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + 10;
	bottom = y + 10;
}

void Item::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	/*if (this->GetState() == CANDLE_STATE_DESTROYED)
	{

	}*/
}

void Item::Render()
{
	animation_set->at(0)->Render(x, y);

	RenderBoundingBox();
}