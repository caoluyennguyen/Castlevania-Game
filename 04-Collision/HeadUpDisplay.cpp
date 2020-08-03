#include <iostream>
#include <fstream>

#include "HeadUpDisplay.h"

HeadUpDisplay* HeadUpDisplay::__instance = NULL;

HeadUpDisplay* HeadUpDisplay::GetInstance()
{
	if (__instance == NULL) __instance = new HeadUpDisplay();
	return __instance;
}

HeadUpDisplay::HeadUpDisplay()
{
	score = 0;
	time = 0;
	bossHp = 0;
	x = 105;
	y = -38;
}

void HeadUpDisplay::LoadResource()
{
	// Init text for head up display
	font = NULL;
	AddFontResourceEx(FILEPATH_FONT, FR_PRIVATE, NULL);

	HRESULT hr = D3DXCreateFont(
		CGame::GetInstance()->GetDirect3DDevice(), 16, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	if (hr != S_OK)
	{
		DebugOut(L"[ERROR] Create font failed\n");
		return;
	}

	headUpDetails = "SCORE-" + to_string(0) + " TIME 0000 STAGEu 00\n";
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

	weaponIcon.push_back(CSprites::GetInstance()->Get(50001));
	weaponIcon.push_back(CSprites::GetInstance()->Get(50001));
	weaponIcon.push_back(CSprites::GetInstance()->Get(60001));
	weaponIcon.push_back(CSprites::GetInstance()->Get(90001));
	weaponIcon.push_back(CSprites::GetInstance()->Get(1001));

	ifstream f;
	f.open(L"hud.txt");
}

void HeadUpDisplay::UnLoadResource()
{
	currentHP.clear();
	loseHP.clear();
	bossHP.clear();
	weaponIcon.clear();
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
	// Simon hp
	for (int i = 0; i < simonHp; i++)
	{
		currentHP[i]->Draw(x + i * 9, y, 255, 0);
	}
	for (int i = 0; i < 16 - simonHp; i++)
	{
		loseHP[i]->Draw(x + (15 - i) * 9, y, 255, 0);
	}
	// Boss hp
	for (int i = 0; i < bossHp; i++)
	{
		bossHP[i]->Draw(x + i * 9, y + 15, 255, 0);
	}
	for (int i = bossHp; i < 16; i++)
	{
		loseHP[i]->Draw(x + (15 - i) * 9, y + 15, 255, 0);
	}

	weapon->Draw(288, -38, 255, 0);

	if (weaponId > 0) weaponIcon[weaponId]->Draw(299, -29, 255, 0);

	str_score = to_string(score);
	while (str_score.length() < 6) str_score = "0" + str_score;

	str_time = to_string(time/1000);
	while (str_time.length() < 4) str_time = "0" + str_time;

	str_scene = to_string(scene);

	str_simonHeart = to_string(simonHeart);
	while (str_simonHeart.length() < 2) str_simonHeart = "0" + str_simonHeart;
}

void HeadUpDisplay::Update(DWORD dt)
{
	time += dt;

	headUpDetails = "SCORE-" + str_score + " TIME " + str_time + " SCENE 0" + str_scene + "\n";
	headUpDetails += "PLAYER                  -" + str_simonHeart + "\n";
	headUpDetails += "ENEMY                   -00\n";
}

