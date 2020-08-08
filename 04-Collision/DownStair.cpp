#include "DownStair.h"
#include "UpStair.h"

DownStair::DownStair(float l, float t, float r, float b, float nx)
{
	x = l;
	y = t;
	width = r - l;
	height = b - t;
	this->nx = nx;
	this->isActive = true;
}

void DownStair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<UpStair*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom)) this->isActive = false;
		}
	}
}

void DownStair::Render()
{
	//RenderBoundingBox();
}

void DownStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

bool DownStair::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float stair_left,
		stair_top,
		stair_right,
		stair_bottom;

	GetBoundingBox(stair_left, stair_top, stair_right, stair_bottom);

	return CGameObject::AABB(stair_left, stair_top, stair_right, stair_bottom, obj_left, obj_top, obj_right, obj_bottom);
}