#pragma once

#pragma region SIMON
#define SIMON_WALKING_SPEED		0.1f 
//0.1f
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING_RIGHT	1
#define SIMON_STATE_WALKING_LEFT	2
#define SIMON_STATE_SIT				3
#define SIMON_STATE_JUMP			4
#define SIMON_STATE_DIE				5
#define SIMON_STATE_HIT_STAND		6
#define SIMON_STATE_HIT_SIT			7

#define SIMON_ANI_IDLE_RIGHT		0
#define SIMON_ANI_IDLE_LEFT			0
#define SIMON_ANI_WALKING_RIGHT		1
#define SIMON_ANI_WALKING_LEFT		1
#define SIMON_ANI_SIT				2
#define SIMON_ANI_JUMP				3
#define SIMON_ANI_DIE				4

#define SIMON_BBOX_WIDTH  64
#define SIMON_BBOX_HEIGHT 60

#define SIMON_UNTOUCHABLE_TIME 5000
#pragma endregion

#define WINDOW_CLASS_NAME L"SampleWindow"
#define MAIN_WINDOW_TITLE L"04 - Collision"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

#define MAX_FRAME_RATE 120

#define ID_TEX_SIMON 0
#define ID_TEX_ENEMY 10
#define ID_TEX_MISC 20