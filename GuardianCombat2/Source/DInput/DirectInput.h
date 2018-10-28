//=============================================================================
//
// ���͏��� [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include <dinput.h>

#define MOUSE_SENSITIVITY			(0.04)
#define MOUSE_SENSITIVITIY_CHANGE	(0.001)

//*****************************************************************************
// �v���g�^�C�v�錾
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
	static const unsigned int LIMIT_COUNT_REPEAT = 20;		//���s�[�g�J�E���g���~�b�^�[

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

	LPDIRECTINPUT8			g_pInput = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;					// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
	BYTE					g_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏�񃏁[�N
	BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̃g���K�[��񃏁[�N
	BYTE					g_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X��񃏁[�N
	BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];			// �L�[�{�[�h�̃��s�[�g��񃏁[�N
	int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g�J�E���^
	bool					g_MouseCursorShow = true;
};

#endif

