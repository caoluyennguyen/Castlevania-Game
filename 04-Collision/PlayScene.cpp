#include <iostream>
#include <fstream>

#include "HeadUpDisplay.h"
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
#include "Ghost.h"
#include "Fleaman.h"
#include "Raven.h"
#include "Skeleton.h"
#include "Zombie.h"
#include "Boss.h"
#include "BreakableWall.h"

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
#define SCENE_SECTION_GRID	7

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
#define OBJECT_TYPE_GHOST	12
#define OBJECT_TYPE_FLEAMAN	13
#define OBJECT_TYPE_RAVEN	14
#define OBJECT_TYPE_SKELETON	15
#define OBJECT_TYPE_ZOMBIE	16
#define OBJECT_TYPE_BOSS	17
#define OBJECT_TYPE_BREAKABLE_WALL	18
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
	mapWidth = atoi(tokens[8].c_str());

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

	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
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

	int id;

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
		case OBJECT_TYPE_SIMON:
			if (player != NULL)
			{
				DebugOut(L"[ERROR] MARIO object was created before!\n");
				return;
			}
			obj = Simon::GetInstance();
			player = (Simon*)obj;
			weapon = new Weapon();
			weapon->SetState(DAGGER_LEFT);
			if (atoi(tokens[4].c_str()) != -1) player->SetState(atoi(tokens[4].c_str()));
			DebugOut(L"[INFO] Player object created!\n");
			break;
		case OBJECT_TYPE_CANDLE:
		{
			obj = new Candle(); 
			id = atoi(tokens[4].c_str());
			obj->SetId(id);
			break;
		}
		case OBJECT_TYPE_GROUND: 
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			id = atoi(tokens[6].c_str());
			obj = new Ground(x, y, r, b);
			obj->SetId(id);
		}
		break;
		case OBJECT_TYPE_ITEM: 
			obj = new Item(atof(tokens[4].c_str()));
			obj->enable = false;
			id = atoi(tokens[5].c_str());
			obj->SetId(id);
			break;
		case OBJECT_TYPE_PORTAL:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			id = atoi(tokens[7].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
			obj->SetId(id);
			break;
		}
		case OBJECT_TYPE_UPSTAIR:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			float nx = atof(tokens[6].c_str());
			int id = atof(tokens[7].c_str());
			obj = new UpStair(x, y, r, b, nx);
			obj->SetId(id);
			break;
		}
		case OBJECT_TYPE_DOWNSTAIR:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			float nx = atof(tokens[6].c_str());
			int id = atof(tokens[7].c_str());
			obj = new DownStair(x, y, r, b, nx);
			obj->SetId(id);
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
			int id = atof(tokens[4].c_str());
			obj = new SmallCandle(0);
			obj->SetId(id);
			break;
		}
		case OBJECT_TYPE_ELEVATOR:
		{
			float minX = atof(tokens[4].c_str());
			float maxX = atof(tokens[5].c_str());
			int id = atof(tokens[6].c_str());
			obj = new Elevator(minX, maxX);
			obj->SetId(id);
			break;
		}
		case OBJECT_TYPE_GHOST:
		{
			obj = new Ghost();
			break;
		}
		case OBJECT_TYPE_FLEAMAN:
			obj = new Fleaman();
			break;
		case OBJECT_TYPE_RAVEN:
			obj = new Raven();
			break;
		case OBJECT_TYPE_SKELETON:
			obj = new Skeleton(atof(tokens[4].c_str()));
			break;
		case OBJECT_TYPE_ZOMBIE:
			obj = new Zombie(atof(tokens[4].c_str()));
			break;
		case OBJECT_TYPE_BOSS:
			obj = new Boss();
			break;
		case OBJECT_TYPE_BREAKABLE_WALL:
		{
			int id = atof(tokens[4].c_str());
			obj = new BreakableWall();
			obj->SetId(id);
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
		player->whip->SetAnimationSet(animation_sets->Get(OBJECT_TYPE_WHIP));
		weapon->SetAnimationSet(animation_sets->Get(OBJECT_TYPE_WEAPON));
	}
	else if (object_type == OBJECT_TYPE_SKELETON)
	{
		Skeleton* skeleton = dynamic_cast<Skeleton*>(obj);
		skeleton->weapon->SetAnimationSet(animation_sets->Get(OBJECT_TYPE_WEAPON));
	}
}

void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	//if (tokens.size() < 2) return; // skip invalid lines - an object set must have at least id, x, y

	LPCWSTR path = ToLPCWSTR(tokens[0]);

	grid = new Grid(path, &objects);
}

void CPlayScene::Load()
{
	flag = true;
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
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
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
			case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	if (!flag) return;

	// Update Simon HP
	HeadUpDisplay::GetInstance()->SetSimonHP(player->GetPlayerHP());
	HeadUpDisplay::GetInstance()->SetWeapon(player->GetWeapon());
	HeadUpDisplay::GetInstance()->SetHeart(player->GetHeart());
	
	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();

	if (cy > game->GetScreenHeight() + 200) CGame::GetInstance()->SwitchScene(CGame::GetInstance()->GetIdCurrentScene());

	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	// Lock camera when fighting boss
	if (cx < 0) cx = 0;
	else if (cx > mapWidth - game->GetScreenWidth()) cx = mapWidth - game->GetScreenWidth();
	if (!player->IsFightingBoss()) CGame::GetInstance()->SetCamPos(int(cx), int(-70.0f) /*cy*/);

	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	coObjects.clear();

	// Load object from grid
	grid->GetListObject(&coObjects);

	// Add player to grid
	coObjects.push_back(player);

	// Add enemy to grid
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isEnemy && objects[i]->enable) coObjects.push_back(objects[i]);
	}

	// Update object
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (objects[i]->isEnemy && !objects[i]->isActive) continue;
		objects[i]->Update(dt, &coObjects);
	}

	if (player == NULL) return;

	if ((weapon->x > cx + SCREEN_WIDTH || weapon->x < cx || weapon->y > cy + SCREEN_HEIGHT) && weapon->isEnable())
	{
		weapon->enable = false;
		player->AddNumOfWeapon();
	}
	else {
		weapon->Update(dt, &coObjects);
	}
}

void CPlayScene::Render()
{
	tilemap->Render(player->x);

	/*for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->enable)
		{
			if (objects[i]->isEnemy && !objects[i]->isActive) objects[i]->RenderActiveBoundingBox();
			else objects[i]->Render();
		}
	}*/

	for (int i = 0; i < coObjects.size(); i++)
	{
		if (coObjects[i]->enable)
		{
			if (coObjects[i]->isEnemy && !coObjects[i]->isActive) {
				//coObjects[i]->RenderActiveBoundingBox();
				continue;
			}
			coObjects[i]->Render();
		}
	}
	
	if (weapon->enable)
	{
		weapon->Render();
		//weapon->RenderBoundingBox();
	}
}

/*
	Unload current scene
*/

void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++) {
		if (!dynamic_cast<Simon*>(objects[i])) delete objects[i];
	}
	objects.clear();
	coObjects.clear();
	player = NULL;
	tilemap = NULL;
	weapon = NULL;
	flag = false;

	if (grid != NULL) grid->Unload();
	grid = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->GetPlayer();
	Weapon* weapon = ((CPlayScene*)scence)->GetWeapon();

	switch (KeyCode)
	{
	case DIK_L:
		simon->Reset();
		break;
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
		simon->Reset();
		break;
	case DIK_Z:
		simon->whip->enable = true;
		simon->whip->StartHit();
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
			simon->whip->animation_set->at(simon->whip_level + 1)->resetAnimation(); // reset to first sprite
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
			simon->whip->animation_set->at(simon->whip_level)->resetAnimation(); // reset to first sprite
		}
		break;
	case DIK_C:
		if (simon->GetState() == SIMON_STATE_HIT_SIT || simon->GetState() == SIMON_STATE_HIT_STAND
			|| simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT
			|| simon->GetState() == SIMON_HIT_DOWNSTAIR_RIGHT || simon->GetState() == SIMON_HIT_DOWNSTAIR_LEFT
			|| simon->GetState() == SIMON_HIT_UPSTAIR_RIGHT || simon->GetState() == SIMON_HIT_UPSTAIR_LEFT
			|| simon->GetHeart() < 1 || simon->GetWeapon() < 0 || simon->GetNumOfWeapon() < 1)
		{
			return;
		}
		simon->SubtractHeart(1);
		simon->SubtractNumOfWeapon();
		simon->isThrowWeapon = true;
		weapon->enable = true;
		weapon->nx = simon->nx;
		weapon->state = simon->GetWeapon();
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
			weapon->vy = -0.4f;
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
		if (simon->nx == 1) {
			weapon->SetState(1);
			simon->SetWeapon(1);
		}
		else {
			weapon->SetState(0);
			simon->SetWeapon(0);
		}
		break;
	case DIK_2:
		weapon->nx = simon->nx;
		weapon->SetState(2);
		simon->SetWeapon(2);
		break;
	case DIK_3:
		weapon->SetState(3);
		simon->SetWeapon(3);
		break;
	case DIK_4:
		weapon->nx = simon->nx;
		weapon->SetState(4);
		simon->SetWeapon(4);
		break;
	case DIK_T:
		CGame::GetInstance()->SwitchScene(1);
		break;
	case DIK_Y:
		CGame::GetInstance()->SwitchScene(2);
		break;
	case DIK_U:
		CGame::GetInstance()->SwitchScene(3);
		break;
	case DIK_I:
		CGame::GetInstance()->SwitchScene(4);
		break;
	case DIK_O:
		CGame::GetInstance()->SwitchScene(5);
		break;
	case DIK_P:
		CGame::GetInstance()->SwitchScene(6);
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	Simon* simon = ((CPlayScene*)scence)->GetPlayer();

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
	if ((simon->GetState() == SIMON_GO_UPSTAIR_RIGHT || simon->GetState() == SIMON_GO_UPSTAIR_LEFT ||
		simon->GetState() == SIMON_GO_DOWNSTAIR_RIGHT || simon->GetState() == SIMON_GO_DOWNSTAIR_LEFT)
		&& simon->animation_set->at(simon->state)->isOver(150) == false)
	{
		return;
	}

	if ((simon->GetState() == SIMON_STATE_INJURED_RIGHT || simon->GetState() == SIMON_STATE_INJURED_LEFT)
		&& simon->animation_set->at(simon->state)->isOver(400) == false)
	{
		return;
	}

	if (simon->GetState() == SIMON_STATE_DIE_RIGHT || simon->GetState() == SIMON_STATE_DIE_LEFT)
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
		/*if (simon->isAbleToMoveToStair && simon->isAbleToStepDownStair) simon->isMoveToStair = true;
		else simon->isMoveToStair = false;*/
		if (simon->isAbleToMoveToStair && simon->isAbleToStepDownStair) simon->isMoveToStairDown = true;
		else simon->isMoveToStairDown = false;

		//if (simon->distanceUp == 0) simon->auto_start = GetTickCount();

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
		/*if (simon->isAbleToMoveToStair && simon->isAbleToStepUpStair) simon->isMoveToStair = true;
		else simon->isMoveToStair = false;*/
		if (simon->isAbleToMoveToStair && simon->isAbleToStepUpStair) simon->isMoveToStairUp = true;
		else simon->isMoveToStairUp = false;

		//if (simon->distanceDown == 0) simon->auto_start = GetTickCount();

		if (simon->isStepOnStair)
		{
			simon->nx = simon->GetNxDownStair();
			simon->isStandUpStair = true;
			simon->isStandDownStair = false;

			if (simon->GetNxDownStair() == -1)
			{
				simon->SetState(SIMON_GO_UPSTAIR_LEFT);
			}
			else simon->SetState(SIMON_GO_UPSTAIR_RIGHT);
		}
	}
	else {
		if (!simon->isMoveToStairUp && !simon->isMoveToStairDown)
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
					if (simon->GetNxDownStair() == -1)
					{
						simon->SetState(SIMON_STAND_ON_UPSTAIR_LEFT);
					}
					else simon->SetState(SIMON_STAND_ON_UPSTAIR_RIGHT);
				}
				else {
					if (simon->GetNxDownStair() == -1)
					{
						simon->SetState(SIMON_STAND_ON_DOWNSTAIR_RIGHT);
					}
					else simon->SetState(SIMON_STAND_ON_DOWNSTAIR_LEFT);
				}
			}
		}
	}
}