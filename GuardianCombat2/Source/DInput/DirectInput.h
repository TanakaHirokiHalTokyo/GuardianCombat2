//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include <dinput.h>

#define MOUSE_SENSITIVITY			(0.04)
#define MOUSE_SENSITIVITIY_CHANGE	(0.001)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

bool InitDinputMouse(HWND hWnd);
void GetMouseState(HWND hWnd);
void UninitDinputMouse();
DIMOUSESTATE ReturnMouseMove();

void SetMouseCursorShow(bool flag);
bool GetMouseCursorShow();


#endif

