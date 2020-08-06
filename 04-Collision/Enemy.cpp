#include "Enemy.h"

Enemy::Enemy() : CGameObject()
{
	hp = 1;
}

void Enemy::Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects)
{
	CGameObject::Update(dt);
}
