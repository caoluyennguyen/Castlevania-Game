#include "Candle.h"

Candle::Candle() : CGameObject()
{
	SetState(CANDLE_STATE_NORMAL);
	//this->item->enable = false;
	//item = new Item(1);
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
	
	if (this->GetState() == CANDLE_STATE_DESTROYED && animation_set->at(CANDLE_STATE_DESTROYED)->isOver(600))
	{
		//this->item->enable = true;
		this->enable = false;
	}
}

void Candle::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	//RenderBoundingBox();
}

void Candle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANDLE_STATE_DESTROYED:
		animation_set->at(CANDLE_STATE_DESTROYED)->resetAnimation();
		animation_set->at(CANDLE_STATE_DESTROYED)->setStartFrameTime(GetTickCount());
		//item->SetPosition(x, y);
		break;
	default:
		break;
	}
}