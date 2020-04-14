#pragma once

class Tilemap
{
	int x;
	int y;

	int numCol;
	int numRow;
public:
	void LoadResource();
	void Render();
};

