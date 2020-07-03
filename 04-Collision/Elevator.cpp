#include "Elevator.h"

Elevator::Elevator(int minX, int maxX) : CGameObject()
{
	this->minX = minX;
	this->maxX = maxX;
	this->vx = 0.05f;
	SetState(ELEVATOR_STATE_NORMAL);
}

void Elevator::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + ELEVATOR_BBOX_WIDTH;
	bottom = y + ELEVATOR_BBOX_HEIGHT;
}

void Elevator::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	x += dx;


	if (this->x < this->minX || this->x > this->maxX)
	{
		this->vx *= -1;
	}
}

void Elevator::Render()
{
	animation_set->at(this->GetState())->Render(x, y);

	RenderBoundingBox();
}

void Elevator::SetState(int state)
{
	CGameObject::SetState(state);
}