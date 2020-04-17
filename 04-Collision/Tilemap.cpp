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

	sprites->Get(8)->Draw(10, 10);
}

void Tilemap::Render()
{
	ifstream f;
	f.open("textures\\map\\test.txt");

	// current resource section flag

	char str[1024];
	while (f.getline(str, 1024))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		vector<string> tokens = split(line);

		//if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

		DebugOut(L"--> %s\n", ToWSTR(line).c_str());

		//for (int i = 0; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
		//{
		//	int sprite_id = atoi(tokens[i].c_str());
		//	int frame_time = atoi(tokens[i + 1].c_str());
		//}
	}

	f.close();
}

Tilemap* Tilemap::GetInstance()
{
	if (_instance == NULL) _instance = new Tilemap();
	return _instance;
}