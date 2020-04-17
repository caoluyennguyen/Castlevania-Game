#pragma once
#include "Utils.h"
#include "Sprites.h"

class Tilemap
{
	Tilemap* _instance;

	int x = 0;				// toa do x cua sprite dau tien tren cung
	int y = 0;				// toa do y cua sprite dau tien tren cung

	int numCol = 8;				// so cot can de ve
	int numRow = 8;				// so hang can de ve

	int mapWidth;			// chieu rong cua map
	int mapHeight;			// chieu cao cua map

	int cellWidth = 32;		// chieu rong cua mot o
	int cellHeight = 32;	// chieu cao cua mot o

	CSprites* sprites;
public:
	Tilemap();
	Tilemap(int mapWidth, int mapHeight);
	void LoadMap();
	void Render();

	Tilemap* GetInstance();
};

