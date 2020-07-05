#include "BlackKnight.h"

BlackKnight::BlackKnight(int minX, int maxX) : CGameObject()
{
	this->minX = minX;
	this->maxX = maxX;
	this->left = rand() % (maxX - minX + 1) + minX;
	this->right = rand() % (maxX - minX + 1) + minX;
	this->vx = 0.05f;
	this->isEnemy = true;
	SetState(BLACKKNIGHT_STATE_WALK_LEFT);
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
	if (this->GetState() == BLACKKNIGHT_STATE_DIE)
	{
		if (animation_set->at(BLACKKNIGHT_STATE_DIE)->isOver(600)) this->enable = false;
		return;
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);
	x += dx;


	if (this->x < this->left)
	{
		this->SetState(BLACKKNIGHT_STATE_WALK_RIGHT);
		this->left = rand() % (maxX - minX + 1) + minX;
		this->right = rand() % (maxX - minX + 1) + minX;
	}
	else if (this->x > this->right)
	{
		this->SetState(BLACKKNIGHT_STATE_WALK_LEFT);
		this->left = rand() % (maxX - minX + 1) + minX;
		this->right = rand() % (maxX - minX + 1) + minX;
	}
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
	case BLACKKNIGHT_STATE_WALK_LEFT:
		vx = -0.04f;
		break;
	case BLACKKNIGHT_STATE_WALK_RIGHT:
		vx = 0.04f;
		break;
	case BLACKKNIGHT_STATE_DIE:
		vx = vy = 0;
		animation_set->at(BLACKKNIGHT_STATE_DIE)->resetAnimation();
		animation_set->at(BLACKKNIGHT_STATE_DIE)->setStartFrameTime(GetTickCount());
		break;
	default:
		break;
	}
}