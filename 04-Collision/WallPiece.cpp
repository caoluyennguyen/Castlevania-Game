#include "WallPiece.h"
#include "Animations.h"

WallPiece::WallPiece()
{
	this->SetAnimationSet(CAnimationSets::GetInstance()->Get(8));
}

void WallPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObject, bool stopMovement)
{
	CGameObject::Update(dt);
	vy += WALLPIECES_GRAVITY * dt;

	x += dx;
	y += dy;
}

void WallPiece::Render()
{
	animation_set->at(0)->Render(x, y);
}

WallPieces::WallPieces(float x, float y)
{
	WallPiece* piece;

	for (int i = 1; i <= 4; i++)
	{
		piece = new WallPiece();
		piece->SetPosition(x, y);

		float vx = (float)(rand() % 200 - 100) / 1000; // rand from -0.1 to 0.2
		float vy = (float)(rand() % 200 - 100) / 1000;
		piece->SetSpeed(vx, vy);

		pieces.push_back(piece);
	}
}

WallPieces::~WallPieces()
{
	for (int i = 0; i < 4; i++)
		delete(pieces[i]);

	pieces.clear();
}

void WallPieces::Update(DWORD dt)
{
	for (auto piece : pieces)
		piece->Update(dt);
}

void WallPieces::Render()
{
	for (auto piece : pieces)
		piece->Render();
}
