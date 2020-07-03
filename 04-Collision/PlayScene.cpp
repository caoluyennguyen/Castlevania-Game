#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "UpStair.h"
#include "DownStair.h"
#include "BlackKnight.h"
#include "VampireBat.h"
#include "SmallCandle.h"
#include "Elevator.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TILEMAP 1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_GROUND	1
#define OBJECT_TYPE_CANDLE	2
#define OBJECT_TYPE_WHIP	3
#define OBJECT_TYPE_ITEM	4
#define OBJECT_TYPE_WEAPON	5
#define OBJECT_TYPE_UPSTAIR	6
#define OBJECT_TYPE_DOWNSTAIR	7
#define OBJECT_TYPE_BLACKKNIGHT	8
#define OBJECT_TYPE_VAMPIREBAT	9
#define OBJECT_TYPE_SMALLCANDLE	10
#define OBJECT_TYPE_ELEVATOR	11

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TILEMAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int pixel = atoi(tokens[0].c_str());
	wstring img_path = ToWSTR(tokens[1]);
	wstring file_path = ToWSTR(tokens[2]);
	int numCol = atoi(tokens[3].c_str());
	int numRow = atoi(tokens[4].c_str());
	int numColToRead = atoi(tokens[5].c_str());
	int numRowToRead = atoi(tokens[6].c_str());
	int idCell = atoi(tokens[7].c_str());

	//CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
	tilemap = new Tilemap(pixel, img_path.c_str(), file_path.c_str(), numCol, numRow, numColToRead, numRowToRead, idCell);
}

void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	/*LPDIRECT3DTEXTURE9 texSimon = CTextures::GetInstance()->Get(ID_TEX_SIMON);
	LPDIRECT3DTEXTURE9 texMisc = CTextures::GetInstance()->Get(ID_TEX_MISC);*/

	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);

	//CSprites::GetInstance()->Add(10001, 0, 0, 60, 64, texSimon); // stand
	//
	//CSprites::GetInstance()->Add(10011, 0, 0, 60, 64, texSimon); // walk
	//CSprites::GetInstance()->Add(10012, 60, 0, 120, 64, texSimon);
	//CSprites::GetInstance()->Add(10013, 120, 0, 180, 64, texSimon);
	//CSprites::GetInstance()->Add(10014, 180, 0, 240, 64, texSimon);
	//
	//CSprites::GetInstance()->Add(10021, 300, 198, 360, 262, texSimon); // sit
	//
	//CSprites::GetInstance()->Add(10031, 240, 0, 300, 64, texSimon); // jump

	//CSprites::GetInstance()->Add(20001, 512, 96, 544, 128, texMisc);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);

	//CAnimations* animations = CAnimations::GetInstance();
	//LPANIMATION ani;
	//ani = new CAnimation(); // Simon idle
	//ani->Add(10001);
	//animations->Add(401, ani);
	//
	//ani = new CAnimation(); // Simon walk
	//ani->Add(10011);
	//ani->Add(10012);
	//ani->Add(10013);
	//ani->Add(10014);
	//animations->Add(402, ani);
	//
	//ani = new CAnimation(); // Simon sit
	//ani->Add(10021);
	//animations->Add(403, ani);
	//
	//ani = new CAnimation(); // Simon jump
	//ani->Add(10031);
	//animations->Add(404, ani);
	

}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
	
	/*LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	LPANIMATION ani = animations->Get(401);
	s->push_back(ani);
	ani = animations->Get(402);
	s->push_back(ani);
	ani = animations->Get(403);
	s->push_back(ani);
	ani = animations->Get(404);
	s->push_back(ani);

	CAnimationSets::GetInstance()->Add(1, s);*/
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
		case OBJECT_TYPE_SIMON:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before! ");
				return;
			}
			obj = new Simon();
			player = (Simon*)obj;
			whip = new Whip();
			weapon = new Weapon();
			weapon->SetState(0);
			break;
		case OBJECT_TYPE_CANDLE: obj = new Candle(); break;
		case OBJECT_TYPE_GROUND: 
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			obj = new Ground(x, y, r, b);
		}
		break;
		case OBJECT_TYPE_ITEM: 
			obj = new Item(atof(tokens[4].c_str()));
			obj->enable = false;
			break;
		case OBJECT_TYPE_PORTAL:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			break;
		}
		case OBJECT_TYPE_UPSTAIR:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			float nx = atof(tokens[6].c_str());
			obj = new UpStair(x, y, r, b, nx);
			break;
		}
		case OBJECT_TYPE_DOWNSTAIR:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			float nx = atof(tokens[6].c_str());
			obj = new DownStair(x, y, r, b, nx);
			break;
		}
		case OBJECT_TYPE_BLACKKNIGHT:
		{
			float minX = atof(tokens[4].c_str());
			float maxX = atof(tokens[5].c_str());
			obj = new BlackKnight(minX, maxX);
			break;
		}
		case OBJECT_TYPE_VAMPIREBAT:
			obj = new VampireBat();
			break;
		case OBJECT_TYPE_SMALLCANDLE:
		{
			float typeOfItem = atof(tokens[4].c_str());
			obj = new SmallCandle(typeOfItem);
			break;
		}
		case OBJECT_TYPE_ELEVATOR:
		{
			float minX = atof(tokens[4].c_str());
			float maxX = atof(tokens[5].c_str());
			obj = new Elevator(minX, maxX);
			break;
		}
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
	}

	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

	if (object_type == OBJECT_TYPE_SIMON)
	{
		whip->SetAnimationSet(animation_sets->Get(OBJECT_TYPE_WHIP));
		weapon->SetAnimationSet(animation_sets->Get(OBJECT_TYPE_WEAPON));
	}
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TILEMAP]") {
			section = SCENE_SECTION_TILEMAP; continue;
		}
		if (line == "[TEXTURES]") {
			section = SCENE_SECTION_TEXTURES; continue;
		}
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') {
			section = SCENE_SECTION_UNKNOWN; continue;
		}
		//
		// data section
		//
		switch (section)
		{
			case SCENE_SECTION_TILEMAP: _ParseSection_TILEMAP(line); break;
			case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
			case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
			case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
			case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);

	//tilemap = new Tilemap();
	//tilemap->LoadMap();

	
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		if (objects[i]->enable)
		{
			coObjects.push_back(objects[i]);
		}
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	if (whip->enable)
	{
		whip->Update(dt, &coObjects);
		whip->SetWhipPosition(cx, cy, player->isStand);
	}
	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (cx < 0)
	{
		cx = 0;
	}
	CGame::GetInstance()->SetCamPos(cx, -70.0f /*cy*/);

	if (weapon->x < cx + SCREEN_WIDTH && weapon->x > cx && weapon->y < cy + SCREEN_HEIGHT && weapon->isEnable())
	{
		coObjects.push_back(player);
		weapon->Update(dt, &coObjects);
	}
	else weapon->enable = false;
}

void CPlayScene::Render()
{
	tilemap->Render(player->x);

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->enable)
		{
			objects[i]->Render();
		}
	}
	if (player->GetState() == SIMON_STATE_HIT_SIT || player->GetState() == SIMON_STATE_HIT_STAND
		|| player->GetState() == SIMON_STATE_HIT_SIT_RIGHT || player->GetState() == SIMON_STATE_HIT_STAND_RIGHT
		|| player->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT || player->GetState() == SIMON_HIT_DOWNSTAIR_LEFT
		|| player->GetState() == SIMON_HIT_UPSTAIR_RIGHT || player->GetState() == SIMON_HIT_UPSTAIR_LEFT)
	{
		if (player->isThrowWeapon == false)
		{
			whip->nx = player->nx;
			whip->Render();
		}
		
	}
	else whip->enable = false;
	if (weapon->enable)
	{
		weapon->Render();
		weapon->RenderBoundingBox();
	}
}

/*
	Unload current scene
*/

void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
	tilemap = NULL;
	flag = false;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->player;
	Whip* whip = ((CPlayScene*)scence)->whip;
	Weapon* weapon = ((CPlayScene*)scence)->weapon;
	switch (KeyCode)
	{
	case DIK_X:
		if (simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_HIT_STAND || simon->GetState() == SIMON_STATE_HIT_SIT
			|| simon->GetState() == SIMON_STATE_JUMP_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT || simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT
			|| simon->isStepOnStair)
		{
			return;
		}
		else {
			if (simon->nx == -1)
			{
				simon->SetState(SIMON_STATE_JUMP);
			}
			else simon->SetState(SIMON_STATE_JUMP_RIGHT);
		}
		break;
	case DIK_A: // reset
		simon->SetState(SIMON_STATE_IDLE_RIGHT);
		simon->SetPosition(100.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	case DIK_Z:
		whip->enable = true;
		whip->StartHit();
		if (simon->GetState() == SIMON_STATE_HIT_SIT || simon->GetState() ==  SIMON_STATE_HIT_STAND
			|| simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT
			|| simon->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT || simon->GetState() == SIMON_HIT_DOWNSTAIR_LEFT)
		{
			return;
		}
		simon->isThrowWeapon = false;
		if (simon->nx == 1)
		{
			if (simon->isStepOnStair)
			{
				simon->SetState(SIMON_HIT_UPSTAIR_RIGHT);
			}
			else if (simon->GetState() == SIMON_STATE_SIT_RIGHT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT_RIGHT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND_RIGHT);
			whip->animation_set->at(1)->resetAnimation();
		}
		else {
			if (simon->isStepOnStair)
			{
				if (simon->isStandDownStair)
				{
					if (simon->GetNxUpStair() == -1) simon->SetState(SIMON_HIT_DOWNSTAIR_LEFT);
					else simon->SetState(SIMON_HIT_DOWNSTAIR_RIGHT);
				}
				else {
					if (simon->GetNxUpStair() == -1) simon->SetState(SIMON_HIT_UPSTAIR_RIGHT);
					else simon->SetState(SIMON_HIT_UPSTAIR_LEFT);
				}
			}
			else if (simon->GetState() == SIMON_STATE_SIT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND);
			whip->animation_set->at(0)->resetAnimation();
		}
		break;
	case DIK_C:
		if (simon->GetState() == SIMON_STATE_HIT_SIT || simon->GetState() == SIMON_STATE_HIT_STAND
			|| simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT
			|| simon->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT || simon->GetState() == SIMON_HIT_DOWNSTAIR_LEFT
			|| simon->GetState() == SIMON_HIT_UPSTAIR_RIGHT || simon->GetState() == SIMON_HIT_UPSTAIR_LEFT
			|| weapon->enable == true)
		{
			return;
		}
		simon->isThrowWeapon = true;
		weapon->enable = true;
		weapon->nx = simon->nx;
		weapon->SetWeaponPosition(simon->x, simon->y, simon->isStand);

		if (weapon->state == 3)
		{
			weapon->StartBoomerangBack();
			weapon->SetSpeed(weapon->nx * 0.2f, 0);
			weapon->SetNxFlyBack(simon->nx);
			weapon->SetVxFlyBack(weapon->vx);
		}
		else if (weapon->state == 2)
		{
			weapon->StartAxeFalling();
			weapon->vx = weapon->nx * 0.1f;
		}
		else if (weapon->state == 4)
		{
			weapon->vx = weapon->nx * 0.2f;
		}
		else weapon->vx = weapon->nx * 0.2f;

		if (simon->isStepOnStair)
		{
			if (simon->isStandDownStair)
			{
				if (simon->GetNxUpStair() == -1) simon->SetState(SIMON_HIT_DOWNSTAIR_LEFT);
				else simon->SetState(SIMON_HIT_DOWNSTAIR_RIGHT);
			}
			else {
				if (simon->GetNxUpStair() == -1) simon->SetState(SIMON_HIT_UPSTAIR_RIGHT);
				else simon->SetState(SIMON_HIT_UPSTAIR_LEFT);
			}
		}
		else if (simon->nx == 1)
		{
			if (simon->GetState() == SIMON_STATE_SIT_RIGHT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT_RIGHT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND_RIGHT);
			if (weapon->GetState() < 2) weapon->SetState(1);
		}
		else {
			if (simon->GetState() == SIMON_STATE_SIT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND);
			if (weapon->GetState() < 2) weapon->SetState(0);
		}
		break;
	case DIK_1:
		if (simon->nx == 1) weapon->SetState(1);
		else weapon->SetState(0);
		break;
	case DIK_2:
		weapon->nx = simon->nx;
		weapon->SetState(2);
		break;
	case DIK_3:
		weapon->SetState(3);
		break;
	case DIK_4:
		weapon->nx = simon->nx;
		weapon->SetState(4);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->player;

	if ((simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_JUMP_RIGHT) && !simon->CheckStandGround())
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_HIT_STAND && simon->animation_set->at(SIMON_STATE_HIT_STAND)->isOver(300) == false)
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_HIT_SIT && simon->animation_set->at(SIMON_STATE_HIT_SIT)->isOver(300) == false)
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT && simon->animation_set->at(SIMON_STATE_HIT_STAND_RIGHT)->isOver(300) == false)
	{
		return;
	}
	if (simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT && simon->animation_set->at(SIMON_STATE_HIT_SIT_RIGHT)->isOver(300) == false)
	{
		return;
	}
	if ((simon->GetState() == SIMON_STATE_GET_ITEM_RIGHT || simon->GetState() == SIMON_STATE_GET_ITEM_LEFT)
		&& simon->animation_set->at(simon->state)->isOver(1000) == false)
	{
		return;
	}
	if ((simon->GetState() == SIMON_HIT_DOWNSTAIR_LEFT || simon->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT)
		&& simon->animation_set->at(simon->state)->isOver(300) == false)
	{
		return;
	}
	if ((simon->GetState() == SIMON_HIT_UPSTAIR_LEFT || simon->GetState() == SIMON_HIT_UPSTAIR_RIGHT)
		&& simon->animation_set->at(simon->state)->isOver(300) == false)
	{
		return;
	}

	//else simon->animation_set->at(simon->GetState())->setStartFrameTime(0);

	// disable control key when Simon die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT)) {
		if (!simon->isStepOnStair)
		{
			simon->nx = 1;
			simon->SetState(SIMON_STATE_WALKING_RIGHT);
		}
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		if (!simon->isStepOnStair)
		{
			simon->nx = -1;
			simon->SetState(SIMON_STATE_WALKING_LEFT);
		}
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (simon->isAbleToMoveToStair && simon->isAbleToStepDownStair) simon->isMoveToStair = true;
		else simon->isMoveToStair = false;

		if (simon->distance == 0) simon->auto_start = GetTickCount();

		if (simon->isStepOnStair)
		{
			simon->nx = simon->GetNxUpStair();
			simon->isStandDownStair = true;
			simon->isStandUpStair = false;

			if (simon->GetNxUpStair() == 1)
			{
				simon->SetState(SIMON_GO_DOWNSTAIR_RIGHT);
			}
			else simon->SetState(SIMON_GO_DOWNSTAIR_LEFT);
		}
		else {
			if (simon->nx == 1)
			{
				simon->SetState(SIMON_STATE_SIT_RIGHT);
			}
			else simon->SetState(SIMON_STATE_SIT);
		}
	}
	else if (game->IsKeyDown(DIK_UP)) {
		if (simon->isAbleToMoveToStair && simon->isAbleToStepUpStair) simon->isMoveToStair = true;
		else simon->isMoveToStair = false;

		if (simon->distance == 0) simon->auto_start = GetTickCount();

		if (simon->isStepOnStair)
		{
			simon->nx = simon->GetNxDownStair();
			simon->isStandUpStair = true;
			simon->isStandDownStair = false;

			if (simon->GetNxDownStair() == 1)
			{
				simon->SetState(SIMON_GO_UPSTAIR_RIGHT);
			}
			else simon->SetState(SIMON_GO_UPSTAIR_LEFT);
		}
	}
	else {
		if (!simon->isMoveToStair)
		{
			if (!simon->isStepOnStair)
			{
				if (simon->nx == 1)
				{
					simon->SetState(SIMON_STATE_IDLE_RIGHT);
				}
				else simon->SetState(SIMON_STATE_IDLE_LEFT);
			}
			else {
				if (simon->isStandUpStair)
				{
					if (simon->GetNxDownStair() == 1)
					{
						simon->SetState(SIMON_STAND_ON_UPSTAIR_RIGHT);
					}
					else simon->SetState(SIMON_STAND_ON_UPSTAIR_LEFT);
				}
				else {
					if (simon->GetNxDownStair() == 1)
					{
						simon->SetState(SIMON_STAND_ON_DOWNSTAIR_LEFT);
					}
					else simon->SetState(SIMON_STAND_ON_DOWNSTAIR_RIGHT);
				}
			}
		}
	}
}