#pragma once
#include <vector>
#include"GameObject.h"

class Cell
{
private:
	int objectId;
	vector<CGameObject*> listObjects;
public:
	void Add(CGameObject* obj) { listObjects.push_back(obj); }
	vector<CGameObject*> GetListObjects() { return listObjects; }
	void Clear() { listObjects.clear(); }
};

typedef Cell* LPCELL;