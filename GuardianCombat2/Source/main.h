#ifndef _MAIN_H_
#define _MAIN_H_

#include <d3d9.h>
#include <d3dx9.h>
#include <math.h>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif // !SAFE_RELEASE

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif // !SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif // !SAFE_DELETE_ARRAY

#define FVF_VERTEX3D	(D3DFVF_XYZ  | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)			//FVF
#define WINDOW_NAME		"Tanaka Project"
#define CLASS_NAME		" "
#define WINDOW_STYLE	WS_OVERLAPPEDWINDOW

constexpr unsigned int ScreenWidth = 1280U;															//画面幅サイズ
constexpr unsigned int ScreenHeight = 720U;															//画面高さサイズ
constexpr int	GameFPS = 60;																	//FPS
constexpr bool	FullScreenFlag = true;


HINSTANCE GetHinstance();
HWND	  GethWnd();
void GameEnd();
double norm(double *vec, int n);
float Randrange(float min, float max, bool bClearRand);
int  Randrange_int(int  min, int  max);

#endif // !_MAIN_H_


