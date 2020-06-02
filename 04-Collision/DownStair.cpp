#include "DownStair.h"

DownStair::DownStair(float l, float t, float r, float b)
{
	x = l;
	y = t;
	width = r - l;
	height = b - t;
}

void DownStair::Render()
{
	RenderBoundingBox();
}

void DownStair::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + width;
	b = y + height;
}