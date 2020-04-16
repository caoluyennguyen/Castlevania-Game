#include <iostream>
#include <fstream>

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"

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

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


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
	//CTextures::GetInstance()->Add(ID_TEX_SIMON, L"textures\\Simon.png", D3DCOLOR_XRGB(255, 255, 255));
	//CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
	//CTextures::GetInstance()->Add(ID_TEX_MISC, L"textures\\Scene1.png", D3DCOLOR_XRGB(176, 224, 248));
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
			break;
		case OBJECT_TYPE_CANDLE: obj = new Candle(); break;
		case OBJECT_TYPE_GROUND: 
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new Ground(x, y, r, b, scene_id);
		}
		break;
		case OBJECT_TYPE_ITEM: obj = new Item(); break;
		case OBJECT_TYPE_PORTAL:
		{
			float r = atof(tokens[4].c_str());
			float b = atof(tokens[5].c_str());
			int scene_id = atoi(tokens[6].c_str());
			obj = new CPortal(x, y, r, b, scene_id);
		}
		break;
		default:
			DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
			return;
	}

	/*CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;
	obj = new Simon();
	player = (Simon*)obj;

	obj->SetPosition(50.0f, 0);

	LPANIMATION_SET ani_set = animation_sets->Get(401);
	ani_set = animation_sets->Get(402);
	ani_set = animation_sets->Get(403);
	ani_set = animation_sets->Get(404);*/
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

	whip->SetAnimationSet(animation_sets->Get(3));
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
}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// Update camera to follow mario
	float cx, cy;
	player->GetPosition(cx, cy);
	whip->SetWhipPosition(cx, cy, true);
	//whip->Update(dt);

	CGame* game = CGame::GetInstance();
	cx -= game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	if ((player->GetState() == SIMON_STATE_HIT_SIT || player->GetState() == SIMON_STATE_HIT_STAND))
	{
		whip->Render();
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
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	Simon* simon = ((CPlayScene*)scence)->player;
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->GetState() == SIMON_STATE_JUMP || simon->GetState() == SIMON_STATE_HIT_STAND || simon->GetState() == SIMON_STATE_HIT_SIT
			|| simon->GetState() == SIMON_STATE_JUMP_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT || simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT)
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
		simon->SetPosition(50.0f, 0.0f);
		simon->SetSpeed(0, 0);
		break;
	case DIK_Z:
		if (simon->GetState() == SIMON_STATE_HIT_SIT || simon->GetState() ==  SIMON_STATE_HIT_STAND
			|| simon->GetState() == SIMON_STATE_HIT_SIT_RIGHT || simon->GetState() == SIMON_STATE_HIT_STAND_RIGHT)
		{
			return;
		}
		if (simon->nx == 1)
		{
			if (simon->GetState() == SIMON_STATE_SIT_RIGHT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT_RIGHT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND_RIGHT);
		}
		else {
			if (simon->GetState() == SIMON_STATE_SIT)
			{
				simon->SetState(SIMON_STATE_HIT_SIT);
			}
			else simon->SetState(SIMON_STATE_HIT_STAND);
		}
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

	//else simon->animation_set->at(simon->GetState())->setStartFrameTime(0);

	// disable control key when Simon die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (game->IsKeyDown(DIK_RIGHT)) {
		simon->nx = 1;
		simon->SetState(SIMON_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT)) {
		simon->nx = -1;
		simon->SetState(SIMON_STATE_WALKING_LEFT);
	}
	else if (game->IsKeyDown(DIK_DOWN)) {
		if (simon->nx == 1)
		{
			simon->SetState(SIMON_STATE_SIT_RIGHT);
		}
		else simon->SetState(SIMON_STATE_SIT);
	}
	else {
		if (simon->nx == 1)
		{
			simon->SetState(SIMON_STATE_IDLE_RIGHT);
		}
		else simon->SetState(SIMON_STATE_IDLE_LEFT);
	}
		
}

void CPlayScene::WhipUpdate(DWORD dt, int cx, int cy)
{}