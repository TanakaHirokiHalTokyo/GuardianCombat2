
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
BOOL Init();			//������
void Uninit(void);													//�I������
void Update(void);													//update
void Draw(void);													//�`��

//------------------------------------------------------------------------------------------------------
//
//		�O���[�o���ϐ�
//
//------------------------------------------------------------------------------------------------------
static HWND g_hWnd;
static bool g_bLoop = true;
static HINSTANCE g_hInstance;
static bool g_bWindw = FullScreenFlag;					//false : FullScreen true : WindowMode
std::random_device rd;

//------------------------------------------------------------------------------------------------------
//
//		WinMain�֐�
//
//------------------------------------------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if defined(_DEBUG) || defined(DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	lpCmdLine = lpCmdLine;
	hPrevInstance = hPrevInstance;			//�x�����

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
	wcex.style = CS_VREDRAW | CS_HREDRAW;						//�c���̍ĕ`�������
	wcex.lpfnWndProc = WndProc;									//�E�B���h�E�v���V�[�W���̖��O
	wcex.cbClsExtra = 0;										//�g��Ȃ�����O��NULL
	wcex.cbWndExtra = 0;										//�g��Ȃ�����O��NULL
	wcex.hInstance = hInstance;									//�C���X�^���X�n���h��
	wcex.hIcon = NULL;											//NULL�Ŗ��Ȃ�
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);					//�}�E�X�J�[�\��
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);			//�w�i�ݒ�
	wcex.lpszMenuName = NULL;									//���j���[�͎g�p���Ȃ��̂�NULL
	wcex.lpszClassName = CLASS_NAME;					//�N���X�o�^��
	wcex.hIconSm = NULL;										//NULL�Ŗ��Ȃ�


	RegisterClassEx(&wcex);

	RECT wr = { 0,0,(LONG)ScreenWidth,(LONG)ScreenHeight };
	AdjustWindowRect(&wr, WINDOW_STYLE, false);			//�o�[�̕����܂߂�WINDOW�T�C�Y�ɂȂ� 800*600 -> 808 * 608

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
		(WINDOW_STYLE /*&~(WS_THICKFRAME | WS_MAXIMIZEBOX)*/),	//WindowsStyle		^���g�����Ƃ�MINIMIZEBOX�𕕈�
		StartWindowLocateX,						//Window�̊J�n���@��X
		StartWindowLocateY,						//Window�̊J�n��  ��Y
		WindowWidth,							//Frame���܂߂���
		WindowHeight,							//Frame���܂߂�����
		NULL,
		NULL,
		hInstance,
		NULL
	);

	UpdateWindow(g_hWnd);
	ShowWindow(g_hWnd, nCmdShow);

	MSG msg;
	//		Device�쐬
	if (!CRendererDirectX::Init(g_hWnd, g_bWindw))
	{
		MessageBoxA(g_hWnd, "�f�o�C�X�쐬�Ɏ��s���܂���", "ERROR", MB_OK);
		return -1;
	}

	//		����������
	if (!Init())
	{
		MessageBoxA(g_hWnd, "�������Ɏ��s���܂���", "ERROR", MB_OK);
		return -1;
	}

	timeBeginPeriod(1);

	//		GameLoop
	do
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{//���b�Z�[�W����
			if (msg.message == WM_QUIT)
			{
				//�Q�[�����[�v�I��
				break;
			}
			else
			{
				//���b�Z�[�W����
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//�Q�[������
			FrameTime = timeGetTime();
			if ((FrameTime - FrameOldTime) >= (1000 / GameFPS))
			{
				Update();
				Draw();
				FrameOldTime = FrameTime;	//OldTime�X�V
			}
		}
	} while (g_bLoop);

	Uninit();
	timeEndPeriod(1);

	return (int)msg.wParam;
}
//------------------------------------------------------------------------------------------------------
//
//		WndProc�֐�
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
			//[ESC]�L�[�������ꂽ
			nID = MessageBoxA(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO | MB_DEFBUTTON2);

			if (nID == IDYES)
			{
				DestroyWindow(hWnd);		//�E�B���h�E��j������
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBoxA(hWnd, "�I�����܂����H", "�I���{�^��", MB_YESNO | MB_DEFBUTTON2);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	case WM_RBUTTONDOWN://�}�E�X�E�N���b�N�������b�Z�[�W
		break;

	case WM_LBUTTONDOWN://�}�E�X���N���b�N�������b�Z�[�W
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
//		����������
//
//------------------------------------------------------------------------------------------------------
BOOL Init()
{
	srand((unsigned int)time(NULL));
	//�@�����K��
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
//		�I������
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
//		�`�揈��
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
