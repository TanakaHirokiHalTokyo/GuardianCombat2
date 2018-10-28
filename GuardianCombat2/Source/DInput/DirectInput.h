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

class DInput
{
public:
	static const unsigned int NUM_KEY_MAX = 256;
	static const unsigned int LIMIT_COUNT_REPEAT = 20;		//リピートカウントリミッター

	HRESULT InitKeyboard(HINSTANCE hInstance,HWND hWnd);
	void UninitKeyboard();
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

private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);
	void UpdateInput(void);

	LPDIRECTINPUT8			g_pInput = NULL;						// DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;					// 入力デバイス(キーボード)へのポインタ
	BYTE					g_aKeyState[NUM_KEY_MAX];				// キーボードの入力情報ワーク
	BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報ワーク
	BYTE					g_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報ワーク
	BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];			// キーボードのリピート情報ワーク
	int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];		// キーボードのリピートカウンタ
	bool					g_MouseCursorShow = true;
};

#endif

