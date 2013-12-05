////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800

#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class Input {
public:
	Input();
	Input(const Input&);
	~Input();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsMousePressed(int);
	bool IsMouseDown(int);
	bool IsKeyPressed(int);
	bool IsKeyDown(int);
	void GetMouseLocation(int&, int&);
	void GetMouseDelta(int&, int&);
	void GetWheelDelta(int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256], m_keyboardOldState[256];
	DIMOUSESTATE m_mouseState, m_mouseOldState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY, m_mouseZ;
	int m_deltaX, m_deltaY, m_deltaZ;

	float m_mouseFactor;
};

#endif