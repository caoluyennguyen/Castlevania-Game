#include "BreakableWall.h"

BreakableWall::BreakableWall()
{
	SetState(NORMAL);
}

void BreakableWall::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject)
{
	if (state == BREAK && isBreakToPieces == false)
	{
		isBreakToPieces = true;
		pieces = new WallPieces(x, y);
	}

	if (pieces != NULL)
		pieces->Update(dt);
}

void BreakableWall::Render()
{
	if (state == NORMAL)
		return;

	if (state == BREAK) animation_set->at(BREAK)->Render(x, y);

	if (isBreakToPieces == true && pieces != NULL)
	{
		if (pieces->GetStartTimeRender() == 0)
			pieces->SetStartTimeRender(GetTickCount());
		else if (GetTickCount() - pieces->GetStartTimeRender() > 2000)
		{
			delete pieces;
			pieces = NULL;
			return;
		}

		pieces->Render();
	}

	RenderBoundingBox();
}

void BreakableWall::SetState(int state)
{
	CGameObject::SetState(state);
}

void BreakableWall::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BREAKABLE_WALL_BBOX_WIDTH;
	bottom = top + BREAKABLE_WALL_BBOX_HEIGHT;
}
