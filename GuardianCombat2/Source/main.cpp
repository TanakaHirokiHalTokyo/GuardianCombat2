
#include "main.h"
#include <random>
#include <time.h>
#include "DirectXRenderer.h"
#include "Game\Object.h"
#include "Imgui\ImguiManager.h"
#include "DInput\DirectInput.h"
#include "XInput\xcontroller.h"
#include "Game\GameManager\GameManager.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL Init();			//初期化
void Uninit(void);													//終了処理
void Update(void);													//update
void Draw(void);													//描画

//------------------------------------------------------------------------------------------------------
//
//		グローバル変数
//
//------------------------------------------------------------------------------------------------------
static HWND g_hWnd;
static bool g_bLoop = true;
static HINSTANCE g_hInstance;
static bool g_bWindw = FullScreenFlag;					//false : FullScreen true : WindowMode
std::random_device rd;

//------------------------------------------------------------------------------------------------------
//
//		WinMain関数
//
//------------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	lpCmdLine = lpCmdLine;
	hPrevInstance = hPrevInstance;			//警告回避

	int WindowWidth = 0;
	int WindowHeight = 0;
	int StartWindowLocateX = 0;
	int StartWindowLocateY = 0;
	DWORD FrameTime = 0;
	DWORD FrameOldTime = 0;
	g_bWindw = FullScreenFlag;

	WNDCLASSEX wcex;

	g_hInstance = hInstance;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_VREDRAW | CS_HREDRAW;						//縦横の再描画をする
	wcex.lpfnWndProc = WndProc;									//ウィンドウプロシージャの名前
	wcex.cbClsExtra = 0;										//使わないから０かNULL
	wcex.cbWndExtra = 0;										//使わないから０かNULL
	wcex.hInstance = hInstance;									//インスタンスハンドル
	wcex.hIcon = NULL;											//NULLで問題なし
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//マウスカーソル
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			//背景設定
	wcex.lpszMenuName = NULL;									//メニューは使用しないのでNULL
	wcex.lpszClassName = CLASS_NAME;					//クラス登録名
	wcex.hIconSm = NULL;										//NULLで問題なし


	RegisterClassEx(&wcex);

	RECT wr = { 0,0,(LONG)ScreenWidth,(LONG)ScreenHeight };
	AdjustWindowRect(&wr, WINDOW_STYLE, false);			//バーの分を含めたWINDOWサイズになる 800*600 -> 808 * 608

	WindowWidth = wr.right - wr.left;
	WindowHeight = wr.bottom - wr.top;

	RECT dr;
	GetWindowRect(GetDesktopWindow(), &dr);

	StartWindowLocateX = (dr.right - WindowWidth) / 2;
	StartWindowLocateY = (dr.bottom - WindowHeight) / 2;

	g_hWnd = CreateWindowExA(
		0,
		CLASS_NAME,
		WINDOW_NAME,
		(WINDOW_STYLE /*&~(WS_THICKFRAME | WS_MAXIMIZEBOX)*/),	//WindowsStyle		^を使うことでMINIMIZEBOXを封印
		StartWindowLocateX,						//Windowの開始時　左X
		StartWindowLocateY,						//Windowの開始時  左Y
		WindowWidth,							//Frameを含めた幅
		WindowHeight,							//Frameを含めた高さ
		NULL,
		NULL,
		hInstance,
		NULL
	);

	UpdateWindow(g_hWnd);
	ShowWindow(g_hWnd, nCmdShow);

	MSG msg;
	//		Device作成
	if (!CRendererDirectX::Init(g_hWnd, g_bWindw))
	{
		MessageBoxA(g_hWnd, "デバイス作成に失敗しました", "ERROR", MB_OK);
		return -1;
	}

	//		初期化処理
	if (!Init())
	{
		MessageBoxA(g_hWnd, "初期化に失敗しました", "ERROR", MB_OK);
		return -1;
	}

	timeBeginPeriod(1);

	//		GameLoop
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{//メッセージ処理
			if (msg.message == WM_QUIT)
			{
				//ゲームループ終了
				break;
			}
			else
			{
				//メッセージ処理
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//ゲーム処理
			FrameTime = timeGetTime();
			if ((FrameTime - FrameOldTime) >= (1000 / GameFPS))
			{
				Update();
				Draw();
				FrameOldTime = FrameTime;	//OldTime更新
			}
		}
	} while (g_bLoop);

	Uninit();
	timeEndPeriod(1);

	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------
//
//		WndProc関数
//
//------------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			//[ESC]キーが押された
			nID = MessageBoxA(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO | MB_DEFBUTTON2);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);		//ウィンドウを破棄する
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBoxA(hWnd, "終了しますか？", "終了ボタン", MB_YESNO | MB_DEFBUTTON2);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	case WM_RBUTTONDOWN://マウス右クリックしたメッセージ
		break;

	case WM_LBUTTONDOWN://マウス左クリックしたメッセージ
		SetFocus(hWnd);
		break;

	default:
		break;
	}
	if (ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//------------------------------------------------------------------------------------------------------
//
//		初期化処理
//
//------------------------------------------------------------------------------------------------------
BOOL Init()
{
	srand((unsigned int)time(NULL));
	//法線正規化
	CRendererDirectX::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	CImGui::Init(GethWnd(), CRendererDirectX::GetDevice());
	InitKeyboard(GetHinstance(), GethWnd());
	InitDinputMouse(g_hWnd);
	X_CONTROLLER::InitXcontroller();
	GameManager::Init();

	return true;
}
//------------------------------------------------------------------------------------------------------
//
//		終了処理
//
//------------------------------------------------------------------------------------------------------
void Uninit(void)
{
	UninitKeyboard();
	UninitDinputMouse();
	X_CONTROLLER::UninitXcontroller();
	GameManager::Uninit();
	CImGui::Uninit();
	CRendererDirectX::Uninit();
}
void Update(void)
{
	CImGui::BeginDraw();
	UpdateKeyboard();
	GetMouseState(g_hWnd);
	X_CONTROLLER::UpdateXcontroller();
	GameManager::Update();
	X_CONTROLLER::UpdateXcontrollerCreateOld();
}
//------------------------------------------------------------------------------------------------------
//
//		描画処理
//
//------------------------------------------------------------------------------------------------------
void Draw(void)
{
	X_CONTROLLER::XcontrollerValue(1);
	CRendererDirectX::DrawBegin();
	GameManager::Draw();
	CImGui::EndDraw();
	CRendererDirectX::DrawEnd();
}
HINSTANCE GetHinstance()
{
	return g_hInstance;
}
HWND GethWnd()
{
	return g_hWnd;
}

void GameEnd()
{
	g_bLoop = false;
}
double norm(double *vec, int n)
{
	int i;
	double s = 0.0;

	for (i = 0; i < n; i++) {
		s += vec[i] * vec[i];
	}

	return sqrt(s);
}

float Randrange(float min, float max, bool bClearRand)
{
	{
		if (bClearRand) {
			std::mt19937 random(rd());
			std::uniform_real_distribution<float> range(min, max);

			return range(random);
		}
		else
		{
			std::mt19937 random;
			std::uniform_real_distribution<float> range(min, max);

			return range(random);
		}
	}
}

int Randrange_int(int  min, int  max)
{
	return rand() % (max - min) + min;
}
