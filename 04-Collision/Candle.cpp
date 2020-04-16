#include "Candle.h"

Candle::Candle()
{
	SetState(CANDLE_STATE_NORMAL);
}

void Candle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANDLE_BBOX_WIDTH;
	bottom = y + CANDLE_BBOX_HEIGHT;
}

void Candle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	/*if (this->GetState() == CANDLE_STATE_DESTROYED)
	{

	}*/
}

void Candle::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
}