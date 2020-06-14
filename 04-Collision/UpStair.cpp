#include "UpStair.h"

UpStair::UpStair(float l, float t, float r, float b, float nx)
{
	x = l;
	y = t;
	width = r - l;
	height = b - t;
	this->nx = nx;
}

void UpStair::Render()
{
	RenderBoundingBox();
}

void UpStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}