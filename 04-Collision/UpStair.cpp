#include "UpStair.h"
#include "DownStair.h"

UpStair::UpStair(float l, float t, float r, float b, float nx)
{
	x = l;
	y = t;
	width = r - l;
	height = b - t;
	this->nx = nx;
	this->isActive = true;
}

void UpStair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	for (int i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<DownStair*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (this->CheckCollision(left, top, right, bottom)) this->isActive = false;
		}
	}
}

void UpStair::Render()
{
	//RenderBoundingBox();
}

void UpStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}

bool UpStair::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float stair_left,
		stair_top,
		stair_right,
		stair_bottom;

	GetBoundingBox(stair_left, stair_top, stair_right, stair_bottom);

	return CGameObject::AABB(stair_left, stair_top, stair_right, stair_bottom, obj_left, obj_top, obj_right, obj_bottom);
}