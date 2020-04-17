#include "Tilemap.h"
#include "Textures.h"
#include <iostream>
#include <fstream>

using namespace std;

Tilemap::Tilemap()
{
	sprites = CSprites::GetInstance();

	LoadMap();
	Render();
}

Tilemap::Tilemap(int mapWidth, int mapHeight)
{ 
	this->mapWidth = mapWidth; 
	this->mapHeight = mapHeight; 
	sprites = CSprites::GetInstance();

	LoadMap();
	Render();
}

void Tilemap::LoadMap()
{
	// Luu tung tile theo id tu 1, 2, ...
	CTextures* textures = CTextures::GetInstance();
	textures->Add(70, L"textures\\map\\Scene1.png", D3DCOLOR_XRGB(255, 0, 255));
	LPDIRECT3DTEXTURE9 texTileMap = textures->Get(70);

	int tileId = 8;
	sprites->Add(tileId, 0, 0, 32, 32, texTileMap);


	ifstream f;
	f.open("textures\\map\\test.txt");

	// current resource section flag
	int i = 0;
	int value;
	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<string> tokens = split(line);

		//if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

		DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		spriteId = new int* [2];
		spriteId[i] = new int[8];
		for (int j = 0; j < tokens.size(); j++)	// why i+=2 ?  sprite_id | frame_time  
		{
			value = atoi(tokens[i].c_str());
			spriteId[i][j] = value;
		}
		i++;
	}

	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			DebugOut(L"--> %s\n", spriteId[i][j]);
		}
	}
	f.close();
}

void Tilemap::Render()
{
	for (int i = 0; i < numCol; i++)
	{
		for (int j = 0; j < numRow; j++)
		{
			sprites->Get(8)->Draw(i*32, j*32);
		}
	}
}

Tilemap* Tilemap::GetInstance()
{
	if (_instance == NULL) _instance = new Tilemap();
	return _instance;
}