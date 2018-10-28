//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include "../main.h"	
#include	<dinput.h>
#include	<d3d9.h>
#include	<d3dx9.h>
#include "DirectInput.h"
//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// �L�[�̍ő吔
#define	LIMIT_COUNT_REPEAT	(20)	// ���s�[�g�J�E���g���~�b�^�[

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;						// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;					// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE					g_aKeyState[NUM_KEY_MAX];				// �L�[�{�[�h�̓��͏�񃏁[�N
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̃g���K�[��񃏁[�N
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̃����[�X��񃏁[�N
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];			// �L�[�{�[�h�̃��s�[�g��񃏁[�N
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];		// �L�[�{�[�h�̃��s�[�g�J�E���^
bool					g_MouseCursorShow = true;

//---------------------------------------------------------------------------------
//		DirectInput�ł̓}�E�X�J�[�\���ł͂Ȃ��A�}�E�X�̈ړ��ʂ��͂��邱�Ƃ��ł���
//---------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8	g_pMouseDevice;		//�}�E�X�f�o�C�X
DIMOUSESTATE			g_MouseData;		//�}�E�X�̏��

//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{

	hWnd = hWnd;		//Warning�΍�

	if(g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	if (g_pInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBoxA(hWnd, "DirectInput�I�u�W�F�N�g�����Ȃ���I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h���Ȃ���I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}
	//===================================================
	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	//===================================================
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)			//DISCL_FOREGROUND�O�ʂɂ��鎞�L�[���͂��󂯎��
	)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
		// �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}

//=============================================================================
// �L�[�{�[�h�̍X�V����
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		//aKetState[DIK_SPACE] & 0x80  <- �L�[��������Ă��邩 �r�b�g������

		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// �L�[���s�[�g���𐶐�
			if(aKeyState[nCnKey])
			{
				if(g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if(g_aKeyStateRepeatCnt[nCnKey] == 1
					|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{// �L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�Ԍo�߂�����L�[���s�[�g���ON
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}
//==============================================================================
//		�}�E�X������
//==============================================================================
bool InitDinputMouse(HWND hWnd)
{
	// �}�E�X�J�[�\���̔�\��
	ShowCursor(g_MouseCursorShow);

	// ���͏����̏�����
	HRESULT ret = S_FALSE;
	if (g_pInput == NULL) {
		return false;
	}


	//�}�E�X�p�Ƀf�o�C�X�I�u�W�F�N�g���쐬
	ret = g_pInput->CreateDevice(GUID_SysMouse, &g_pMouseDevice, NULL);
	if (FAILED(ret))
	{
		//�f�o�C�X�̍쐬�Ɏ��s
		return false;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	ret = g_pMouseDevice->SetDataFormat(&c_dfDIMouse);	// �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
	if (FAILED(ret)) {
		// �f�[�^�t�H�[�}�b�g�Ɏ��s
		return false;
	}
	// ���[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	ret = g_pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// ���[�h�̐ݒ�Ɏ��s
		return false;
	}

	// �f�o�C�X�̐ݒ�
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// ���Βl���[�h�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j

	ret = g_pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// �f�o�C�X�̐ݒ�Ɏ��s
		return false;
	}

	// ���͐���J�n
	g_pMouseDevice->Acquire();

	return true;

}
//============================================================================
//	DirectInput�̃}�E�X�f�o�C�X��Ԏ擾����
//============================================================================
void GetMouseState(HWND hWnd)
{
	if (g_pMouseDevice == NULL) {
		// �I�u�W�F�N�g�����O�ɌĂ΂ꂽ�Ƃ��͂����Ő���������
		InitDinputMouse(hWnd);
	}

	// ���͏����̏�����
	if (g_pMouseDevice == NULL)
	{
		MessageBoxA(hWnd, "�}�E�X�f�o�C�X���Ȃ���I", "�x���I", MB_ICONWARNING);
	}

	// �ǎ�O�̒l��ێ����܂�
	DIMOUSESTATE g_zdiMouseState_bak;	// �}�E�X���(�ω����m�p)
	memcpy(&g_zdiMouseState_bak, &g_MouseData, sizeof(g_zdiMouseState_bak));

	// �}�E�X�̏�Ԃ��擾���܂�
	HRESULT	hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseData);
	if (hr == DIERR_INPUTLOST) {
		
		g_pMouseDevice->Unacquire();
		// DirectInput�̃f�o�C�X���J��
		if (g_pMouseDevice) {
			g_pMouseDevice->Release();
			g_pMouseDevice = NULL;
		}
		// ���͏����̏�����
		if (g_pMouseDevice == NULL)
		{
			MessageBoxA(hWnd, "�}�E�X���ؒf����܂����B\n �}�E�X���Đڑ����܂�", "�x���I", MB_ICONWARNING);
		}
		InitDinputMouse(hWnd);

		hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseData);
		
	}
	

}
//==============================================================================
//		�}�E�X�I������
//==============================================================================
void UninitDinputMouse()
{
	// DirectInput�̃f�o�C�X���J��
	if (g_pMouseDevice) {
		g_pMouseDevice->Unacquire();
		g_pMouseDevice->Release();
		g_pMouseDevice = NULL;
	}

	UninitInput();
}
DIMOUSESTATE ReturnMouseMove()
{
	return g_MouseData;
}

void SetMouseCursorShow(bool flag)
{
	g_MouseCursorShow = flag;
	ShowCursor(g_MouseCursorShow);
}

bool GetMouseCursorShow()
{
	return g_MouseCursorShow;
}

HRESULT DInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// ���͏����̏�����
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBoxA(hWnd, "DirectInput�I�u�W�F�N�g�����Ȃ���I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�o�C�X�̍쐬
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h���Ȃ���I", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}
	//===================================================
	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	//===================================================
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)			//DISCL_FOREGROUND�O�ʂɂ��鎞�L�[���͂��󂯎��
	)))
	{
		MessageBoxA(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

void DInput::UninitKeyboard()
{
	if (g_pDevKeyboard != NULL)
	{// ���̓f�o�C�X(�L�[�{�[�h)�̊J��
	 // �L�[�{�[�h�ւ̃A�N�Z�X�����J��(���͐���I��)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// ���͏����̏I������
	UninitInput();
}


void DInput::UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// �f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		//aKetState[DIK_SPACE] & 0x80  <- �L�[��������Ă��邩 �r�b�g������

		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// �L�[�g���K�[�E�����[�X���𐶐�
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// �L�[���s�[�g���𐶐�
			if (aKeyState[nCnKey])
			{
				if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if (g_aKeyStateRepeatCnt[nCnKey] == 1
						|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{// �L�[�������n�߂��ŏ��̃t���[���A�܂��͈�莞�Ԍo�߂�����L�[���s�[�g���ON
						g_aKeyStateRepeat[nCnKey] = aKeyState[nCnKey];
					}
					else
					{
						g_aKeyStateRepeat[nCnKey] = 0;
					}
				}
			}
			else
			{
				g_aKeyStateRepeatCnt[nCnKey] = 0;
				g_aKeyStateRepeat[nCnKey] = 0;
			}

			// �L�[�v���X����ۑ�
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		g_pDevKeyboard->Acquire();
	}
}

bool DInput::GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

HRESULT DInput::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;		//Warning�΍�

	if (g_pInput == NULL)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void DInput::UninitInput(void)
{
	if (g_pInput != NULL)
	{// DirectInput�I�u�W�F�N�g�̊J��
		g_pInput->Release();
		g_pInput = NULL;
	}
}

void DInput::UpdateInput(void)
{
}
