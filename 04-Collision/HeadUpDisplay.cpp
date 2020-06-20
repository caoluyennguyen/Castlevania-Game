#include "HeadUpDisplay.h"

HeadUpDisplay* HeadUpDisplay::__instance = NULL;

HeadUpDisplay* HeadUpDisplay::GetInstance(CGame* game)
{
	if (__instance == NULL) __instance = new HeadUpDisplay(game);
	return __instance;
}

HeadUpDisplay::HeadUpDisplay(CGame *game)
{
	this->game = game;
	time = 0;
	x = 105;
	y = -38;
}

void HeadUpDisplay::LoadResource()
{
	// Init text for head up display
	font = NULL;
	AddFontResourceEx(FILEPATH_FONT, FR_PRIVATE, NULL);

	HRESULT hr = D3DXCreateFont(
		game->GetDirect3DDevice(), 16, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] Create font failed\n");
		return;
	}

	headUpDetails = "SCORE-000000 TIME 0000 SCENE 00\n";
	headUpDetails += "PLAYER                  -00\n";
	headUpDetails += "ENEMY                   -00\n";

	// Init image for head up display
	for (int i = 0; i < 16; i++)
	{
		current = new CSprite(100, 0, 0, 8, 15, CTextures::GetInstance()->Get(104));
		currentHP.push_back(current);

		lose = new CSprite(101, 8, 0, 16, 15, CTextures::GetInstance()->Get(104));
		loseHP.push_back(lose);

		boss = new CSprite(102, 16, 0, 24, 15, CTextures::GetInstance()->Get(104));
		bossHP.push_back(boss);
	}

	weapon = new CSprite(103, 0, 0, 95, 40, CTextures::GetInstance()->Get(105));
}

void HeadUpDisplay::UnLoadResource()
{
	currentHP.clear();
	loseHP.clear();
	bossHP.clear();
}

void HeadUpDisplay::Render()
{
	// Draw text
	RECT rect;
	SetRect(&rect, 0, 15, SCREEN_WIDTH, 80);

	if (font != NULL)
	{
		font->DrawTextA(NULL, headUpDetails.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
	}
	
	//Draw image
	for (int i = 0; i < 8; i++)
	{
		currentHP[i]->Draw(x + i * 9 + game->GetCamPosX(), y);
		loseHP[i]->Draw(x + (15-i) * 9 + game->GetCamPosX(), y);
		bossHP[i]->Draw(x + i * 9 + game->GetCamPosX(), y + 15);
		loseHP[i]->Draw(x + (15 - i) * 9 + game->GetCamPosX(), y + 15);
	}

	weapon->Draw(288 + game->GetCamPosX(), -38);
}

void HeadUpDisplay::Update(DWORD dt)
{
	time += dt;

	headUpDetails = "SCORE-000000 TIME " + to_string(time/1000) + " SCENE 00" + "\n";
	headUpDetails += "PLAYER                  -00\n";
	headUpDetails += "ENEMY                   -00\n";
}

