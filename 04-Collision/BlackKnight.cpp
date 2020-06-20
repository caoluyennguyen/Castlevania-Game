#include "BlackKnight.h"

BlackKnight::BlackKnight() : CGameObject()
{
	SetState(BLACKKNIGHT_STATE_NORMAL);
}

void BlackKnight::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLACKKNIGHT_BBOX_WIDTH;
	bottom = y + BLACKKNIGHT_BBOX_HEIGHT;
}

void BlackKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	
}

void BlackKnight::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
}

void BlackKnight::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BLACKKNIGHT_STATE_WALK:
		//animation_set->at(BLACKKNIGHT_STATE_WALK)->resetAnimation();
		//animation_set->at(BLACKKNIGHT_STATE_WALK)->setStartFrameTime(GetTickCount());
	default:
		break;
	}
}