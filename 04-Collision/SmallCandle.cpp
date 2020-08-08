#include "SmallCandle.h"

SmallCandle::SmallCandle(int TypeOfItem) : CGameObject()
{
	this->typeOfItem = TypeOfItem;
	SetState(SMALL_CANDLE_STATE_NORMAL);
}

void SmallCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	right = x + SMALL_CANDLE_BBOX_WIDTH;
	bottom = y + SMALL_CANDLE_BBOX_HEIGHT;
	top = bottom - 25;
}

void SmallCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	if (this->GetState() == SMALL_CANDLE_STATE_DESTROYED && animation_set->at(SMALL_CANDLE_STATE_DESTROYED)->isOver(600))
	{
		this->enable = false;
	}
}

void SmallCandle::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	//RenderBoundingBox();
}

void SmallCandle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SMALL_CANDLE_STATE_DESTROYED:
		animation_set->at(SMALL_CANDLE_STATE_DESTROYED)->resetAnimation();
		animation_set->at(SMALL_CANDLE_STATE_DESTROYED)->setStartFrameTime(GetTickCount());
	default:
		break;
	}
}