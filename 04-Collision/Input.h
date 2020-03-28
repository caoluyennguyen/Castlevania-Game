#pragma once
#include "Game.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 1024

//class CSampleKeyHander : public CKeyEventHandler
//{
//	virtual void KeyState(BYTE* states);
//	virtual void OnKeyDown(int KeyCode);
//	virtual void OnKeyUp(int KeyCode);
//};
//
//CSampleKeyHander* keyHandler;
//

/*
Abstract class to define keyboard event handlers
*/
class CKeyEventHandler
{
public:
	virtual void KeyState(BYTE *state) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
};

typedef CKeyEventHandler * LPKEYEVENTHANDLER;

class Input
{
	static Input* __instance;

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;
public:
	int IsKeyDown(int KeyCode);

	void InitKeyboard(LPKEYEVENTHANDLER handler, HWND hWnd);
	void ProcessKeyboard();

	static Input* GetInstance();
};