#pragma once

#pragma region SIMON
#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE_LEFT			0
#define SIMON_STATE_WALKING_LEFT		1
#define SIMON_STATE_WALKING_RIGHT		2
#define SIMON_STATE_SIT					3
#define SIMON_STATE_JUMP				4
#define SIMON_STATE_DIE					5
#define SIMON_STATE_HIT_STAND			6
#define SIMON_STATE_HIT_SIT				7
#define SIMON_STATE_IDLE_RIGHT			8
#define SIMON_STATE_SIT_RIGHT			9
#define SIMON_STATE_JUMP_RIGHT			10
#define SIMON_STATE_DIE_RIGHT			11
#define SIMON_STATE_HIT_STAND_RIGHT		12
#define SIMON_STATE_HIT_SIT_RIGHT		13
#define SIMON_STATE_GET_ITEM_LEFT		14
#define SIMON_STATE_GET_ITEM_RIGHT		15

//#define SIMON_ANI_IDLE_RIGHT		0
//#define SIMON_ANI_WALKING_RIGHT		1
//#define SIMON_ANI_SIT_RIGHT			2
//#define SIMON_ANI_JUMP_RIGHT		3
//#define SIMON_ANI_DIE_RIGHT			4
//#define SIMON_ANI_IDLE_LEFT			5
//#define SIMON_ANI_WALKING_LEFT		6
//#define SIMON_ANI_SIT_RIGHT			7
//#define SIMON_ANI_JUMP_RIGHT		8
//#define SIMON_ANI_DIE_RIGHT			9

#define SIMON_BBOX_WIDTH  64
#define SIMON_BBOX_HEIGHT 60

#define SIMON_UNTOUCHABLE_TIME 5000
#pragma endregion

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 512
#define SCREEN_HEIGHT 480

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20