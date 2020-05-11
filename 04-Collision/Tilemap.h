#pragma once
#include "Utils.h"
#include "Sprites.h"

class Tilemap
{
	Tilemap* _instance;

	int x = 0;				// toa do x cua sprite dau tien tren cung
	int y = 0;				// toa do y cua sprite dau tien tren cung

	int numCol = 48;		// so cot can de ve
	int numRow = 10;		// so hang can de ve

	int mapWidth;			// chieu rong cua map
	int mapHeight;			// chieu cao cua map

	int cellWidth = 32;		// chieu rong cua mot o
	int cellHeight = 32;	// chieu cao cua mot o

	vector<vector<int>> cellId;

	CSprites* sprites;
public:
	Tilemap();
	void LoadMap();
	void Render();
	void Render(int x);

	Tilemap* GetInstance();
};

