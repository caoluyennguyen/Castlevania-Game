#include "Tilemap.h"
#include "Textures.h"
#include "Simon.h"
#include <iostream>
#include <fstream>

using namespace std;

Tilemap::Tilemap(int pixel, LPCWSTR bgImagePath, LPCWSTR filePath, int numCol, int numRow, int numColToRead, int numRowToRead, int idCell)
{
	sprites = CSprites::GetInstance();
	this->pixel = pixel;
	this->bgImagePath = bgImagePath;
	this->filePath = filePath;
	this->numCol = numCol;
	this->numRow = numRow;
	this->numColToRead = numColToRead;
	this->numRowToRead = numRowToRead;
	this->idCell = idCell;

	LoadMap();
}

void Tilemap::LoadMap()
{
	// Luu tung tile theo id tu 1, 2, ...
	CTextures* textures = CTextures::GetInstance();
	textures->Add(70, bgImagePath, D3DCOLOR_XRGB(255, 0, 255));
	//textures->Add(70, L"textures\\map\\Scene1.png", D3DCOLOR_XRGB(255, 0, 255));
	//textures->Add(70, L"textures\\map\\Scene2.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texTileMap = textures->Get(70);

	//idCell = 1;
	//idCell = 0;
	for (int i = 0; i < numRowToRead; i++)
	//for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < numColToRead; j++)
		//for (int j = 0; j < 7; j++)
		{
			sprites->Add(idCell, pixel * j, pixel * i, pixel + pixel * j, pixel + pixel * i, texTileMap);
			//sprites->Add(idCell, 32 * j, 32 * i, 32 + 32 * j, 32 + 32 * i, texTileMap);
			//sprites->Add(idCell, 48 * j, 48 * i, 48 + 48 * j, 48 + 48 * i, texTileMap);
			idCell++;
		}
	}

	ifstream f;
	f.open(filePath);
	//f.open("textures\\map\\Scene1.txt");
	//f.open("textures\\map\\Scene2.txt");

	// current resource section flag
	int value;
	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<string> tokens = split(line, " ");

		//if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time
		//spriteId = new int* [2];
		//spriteId[i] = new int[8];
		//for (int j = 0; j < tokens.size(); j++)	// why i+=2 ?  sprite_id | frame_time  
		//{
		//	value = atoi(tokens[i].c_str());
		//	spriteId[i][j] = value;
		//}
		//i++;
		vector<int> lineOfCell;
		DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		for (int i = 0; i < tokens.size(); i++)	// why i+=2 ?  sprite_id | frame_time  
		{
			if (atoi(tokens[i].c_str())) lineOfCell.push_back(atoi(tokens[i].c_str()));
		}
		cellId.push_back(lineOfCell);
	}

	f.close();
}

void Tilemap::Render()
{
	for (int i = 0; i < numRow; i++)
	{
		for (int j = 0; j < numCol; j++)
		{
			sprites->Get(cellId[i][j])->Draw(j*pixel, i*pixel);
			//sprites->Get(cellId[i][j])->Draw(j*32, i*32);
			//sprites->Get(cellId[i][j])->Draw(j*48, i*48);
		}
	}
}

void Tilemap::Render(int x)
{
	int start;
	int finish;
	if (Simon::GetInstance()->IsFightingBoss()) {
		start = 23;
		finish = 48;
	}
	else {
		start = x / pixel - 12;
		finish = start + 29;
	}

	if (start < 0)
	{
		start = 0;
	}
	if (finish > numCol)
	{
		finish = numCol;
	}
	for (int i = 0; i < numRow; i++)
	{
		for (int j = start; j < finish; j++)
		{
			sprites->Get(cellId[i][j])->Draw(j*pixel, i*pixel);
		}
	}
}

Tilemap* Tilemap::GetInstance()
{
	if (_instance == NULL) _instance = new Tilemap();
	return _instance;
}