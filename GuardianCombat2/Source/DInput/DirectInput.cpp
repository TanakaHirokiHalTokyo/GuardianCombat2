//=============================================================================
//
// 入力処理 [input.cpp]
//
//=============================================================================
#include "../main.h"	
#include	<dinput.h>
#include	<d3d9.h>
#include	<d3dx9.h>
#include "DirectInput.h"
//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)	// キーの最大数
#define	LIMIT_COUNT_REPEAT	(20)	// リピートカウントリミッター

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pInput = NULL;						// DirectInputオブジェクトへのポインタ
LPDIRECTINPUTDEVICE8	g_pDevKeyboard = NULL;					// 入力デバイス(キーボード)へのポインタ
BYTE					g_aKeyState[NUM_KEY_MAX];				// キーボードの入力情報ワーク
BYTE					g_aKeyStateTrigger[NUM_KEY_MAX];		// キーボードのトリガー情報ワーク
BYTE					g_aKeyStateRelease[NUM_KEY_MAX];		// キーボードのリリース情報ワーク
BYTE					g_aKeyStateRepeat[NUM_KEY_MAX];			// キーボードのリピート情報ワーク
int						g_aKeyStateRepeatCnt[NUM_KEY_MAX];		// キーボードのリピートカウンタ
bool					g_MouseCursorShow = true;

//---------------------------------------------------------------------------------
//		DirectInputではマウスカーソルではなく、マウスの移動量をはかることができる
//---------------------------------------------------------------------------------
LPDIRECTINPUTDEVICE8	g_pMouseDevice;		//マウスデバイス
DIMOUSESTATE			g_MouseData;		//マウスの状態

//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd)
{

	hWnd = hWnd;		//Warning対策

	if(g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
										IID_IDirectInput8, (void**)&g_pInput, NULL)))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	if (g_pInput != NULL)
	{// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// 入力処理の初期化
	if(FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBoxA(hWnd, "DirectInputオブジェクトが作れないよ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if(FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBoxA(hWnd, "キーボードがないよ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if(FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBoxA(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	//===================================================
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	//===================================================
	if(FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)			//DISCL_FOREGROUND前面にある時キー入力を受け取る
	)))
	{
		MessageBoxA(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
		// キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if(SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		//aKetState[DIK_SPACE] & 0x80  <- キーが押されているか ビットが立つ

		for(int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// キーリピート情報を生成
			if(aKeyState[nCnKey])
			{
				if(g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if(g_aKeyStateRepeatCnt[nCnKey] == 1
					|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{// キーを押し始めた最初のフレーム、または一定時間経過したらキーリピート情報ON
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

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int nKey)
{
	return (g_aKeyStateRepeat[nKey] & 0x80) ? true: false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true: false;
}
//==============================================================================
//		マウス初期化
//==============================================================================
bool InitDinputMouse(HWND hWnd)
{
	// マウスカーソルの非表示
	ShowCursor(g_MouseCursorShow);

	// 入力処理の初期化
	HRESULT ret = S_FALSE;
	if (g_pInput == NULL) {
		return false;
	}


	//マウス用にデバイスオブジェクトを作成
	ret = g_pInput->CreateDevice(GUID_SysMouse, &g_pMouseDevice, NULL);
	if (FAILED(ret))
	{
		//デバイスの作成に失敗
		return false;
	}
	// データフォーマットを設定
	ret = g_pMouseDevice->SetDataFormat(&c_dfDIMouse);	// マウス用のデータ・フォーマットを設定
	if (FAILED(ret)) {
		// データフォーマットに失敗
		return false;
	}
	// モードを設定（フォアグラウンド＆非排他モード）
	ret = g_pMouseDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(ret)) {
		// モードの設定に失敗
		return false;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	// 相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	ret = g_pMouseDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph);
	if (FAILED(ret)) {
		// デバイスの設定に失敗
		return false;
	}

	// 入力制御開始
	g_pMouseDevice->Acquire();

	return true;

}
//============================================================================
//	DirectInputのマウスデバイス状態取得処理
//============================================================================
void GetMouseState(HWND hWnd)
{
	if (g_pMouseDevice == NULL) {
		// オブジェクト生成前に呼ばれたときはここで生成させる
		InitDinputMouse(hWnd);
	}

	// 入力処理の初期化
	if (g_pMouseDevice == NULL)
	{
		MessageBoxA(hWnd, "マウスデバイスがないよ！", "警告！", MB_ICONWARNING);
	}

	// 読取前の値を保持します
	DIMOUSESTATE g_zdiMouseState_bak;	// マウス情報(変化検知用)
	memcpy(&g_zdiMouseState_bak, &g_MouseData, sizeof(g_zdiMouseState_bak));

	// マウスの状態を取得します
	HRESULT	hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseData);
	if (hr == DIERR_INPUTLOST) {
		
		g_pMouseDevice->Unacquire();
		// DirectInputのデバイスを開放
		if (g_pMouseDevice) {
			g_pMouseDevice->Release();
			g_pMouseDevice = NULL;
		}
		// 入力処理の初期化
		if (g_pMouseDevice == NULL)
		{
			MessageBoxA(hWnd, "マウスが切断されました。\n マウスを再接続します", "警告！", MB_ICONWARNING);
		}
		InitDinputMouse(hWnd);

		hr = g_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &g_MouseData);
		
	}
	

}
//==============================================================================
//		マウス終了処理
//==============================================================================
void UninitDinputMouse()
{
	// DirectInputのデバイスを開放
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
	// 入力処理の初期化
	if (FAILED(InitInput(hInstance, hWnd)))
	{
		MessageBoxA(hWnd, "DirectInputオブジェクトが作れないよ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// デバイスの作成
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		MessageBoxA(hWnd, "キーボードがないよ！", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBoxA(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}
	//===================================================
	// 協調モードを設定（フォアグラウンド＆非排他モード）
	//===================================================
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)			//DISCL_FOREGROUND前面にある時キー入力を受け取る
	)))
	{
		MessageBoxA(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDevKeyboard->Acquire();

	return S_OK;
}

void DInput::UninitKeyboard()
{
	if (g_pDevKeyboard != NULL)
	{// 入力デバイス(キーボード)の開放
	 // キーボードへのアクセス権を開放(入力制御終了)
		g_pDevKeyboard->Unacquire();

		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// 入力処理の終了処理
	UninitInput();
}


void DInput::UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];

	// デバイスからデータを取得
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), aKeyState)))
	{
		//aKetState[DIK_SPACE] & 0x80  <- キーが押されているか ビットが立つ

		for (int nCnKey = 0; nCnKey < NUM_KEY_MAX; nCnKey++)
		{
			// キートリガー・リリース情報を生成
			g_aKeyStateTrigger[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & aKeyState[nCnKey];
			g_aKeyStateRelease[nCnKey] = (g_aKeyState[nCnKey] ^ aKeyState[nCnKey]) & g_aKeyState[nCnKey];

			// キーリピート情報を生成
			if (aKeyState[nCnKey])
			{
				if (g_aKeyStateRepeatCnt[nCnKey] < LIMIT_COUNT_REPEAT)
				{
					g_aKeyStateRepeatCnt[nCnKey]++;
					if (g_aKeyStateRepeatCnt[nCnKey] == 1
						|| g_aKeyStateRepeatCnt[nCnKey] >= LIMIT_COUNT_REPEAT)
					{// キーを押し始めた最初のフレーム、または一定時間経過したらキーリピート情報ON
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

			// キープレス情報を保存
			g_aKeyState[nCnKey] = aKeyState[nCnKey];
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDevKeyboard->Acquire();
	}
}

bool DInput::GetKeyboardPress(int nKey)
{
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

HRESULT DInput::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	hWnd = hWnd;		//Warning対策

	if (g_pInput == NULL)
	{
		// DirectInputオブジェクトの作成
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
	{// DirectInputオブジェクトの開放
		g_pInput->Release();
		g_pInput = NULL;
	}
}

void DInput::UpdateInput(void)
{
}
