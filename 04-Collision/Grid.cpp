#include "Grid.h"
#include "Utils.h"
#include "Game.h"
#include <iostream>
#include <fstream>

Grid::Grid(LPCWSTR filePath, vector<LPGAMEOBJECT>* listObject)
{
	this->listObject = listObject;
	DebugOut(L"[INFO] Start loading grid resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
		case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
		case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

void Grid::_ParseSection_SETTINGS(string line)
{
	vector<string> tokens = split(line);
	DebugOut(L"--> %s\n", ToWSTR(line).c_str());

	cellWidth = atoi(tokens[0].c_str());
	numCol = atoi(tokens[1].c_str());
	numRow = atoi(tokens[2].c_str());

	cells = new LPCELL[numCol];
	for (int i = 0; i < numCol; i++)
		cells[i] = new Cell[numRow];
}

void Grid::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return; // skip invalid lines

	int cellX = atoi(tokens[0].c_str());
	int cellY = atoi(tokens[1].c_str());
	int objectId;

	for (int i = 2; i < tokens.size(); i++)
	{
		objectId = atoi(tokens[i].c_str());
		cells[cellX][cellY].Add(listObject->at(objectId));
	}
}

void Grid::Load(LPCWSTR filePath, vector<LPGAMEOBJECT> *listObject)
{
	this->listObject = listObject;
	DebugOut(L"[INFO] Start loading grid resources from : %s \n", filePath);

	ifstream f;
	f.open(filePath);

	// current resource section flag
	int section;

	char str[MAX_GRID_LINE];
	while (f.getline(str, MAX_GRID_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SETTINGS]") {
			section = GRID_SECTION_SETTINGS; continue;
		}
		if (line == "[OBJECTS]") {
			section = GRID_SECTION_OBJECTS; continue;
		}
		//
		// data section
		//
		switch (section)
		{
			case GRID_SECTION_SETTINGS: _ParseSection_SETTINGS(line); break;
			case GRID_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene resources %s\n", filePath);
}

void Grid::GetListObject(vector<LPGAMEOBJECT> *listObject)
{
	listObject->clear();

	int left, top, right, bottom;
	int i, j, k;
	left = CGame::GetInstance()->GetCamPosX() / cellWidth;
	top = CGame::GetInstance()->GetCamPosY() / cellWidth;
	right = (int)(CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()) / cellWidth
		+ ((int)(CGame::GetInstance()->GetCamPosX() + CGame::GetInstance()->GetScreenWidth()) % cellWidth ? 1 : 0);
	bottom = (int)(CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight()) / cellWidth
		+ ((int)(CGame::GetInstance()->GetCamPosY() + CGame::GetInstance()->GetScreenHeight()) % cellWidth ? 1 : 0);

	LPGAMEOBJECT obj;

	if (right < 0 || left > numCol || bottom < 0 && top > numRow)
	{
		return;
	}

	if (right > numCol)
	{
		right = numCol;
	}
	if (bottom > numRow)
	{
		bottom = numRow;
	}
	if (left < 0)
	{
		left = 0;
	}
	if (top > 0)
	{
		top = 0;
	}

	LPCELL cell;
	for (i = left; i < right; i++)
	{
		for (j = top; j < bottom; j++)
		{
			cell = &cells[i][j];
			if (cell)
			{
				if (cells[i][j].GetListObjects().size() != 0)
				{
					for (k = 0; k < cells[i][j].GetListObjects().size(); k++)
					{
						obj = cells[i][j].GetListObjects().at(k);
						/*if (!checkContainId(list_object, e)) {
							list_object->push_back(e);
						}*/

						listObject->push_back(obj);
					}
				}
			}

		}
	}
}

void Grid::Unload()
{

}