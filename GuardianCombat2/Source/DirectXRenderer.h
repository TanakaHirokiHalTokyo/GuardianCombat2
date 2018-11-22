#pragma once

#include "main.h"

class CRendererDirectX
{
private:
	static LPDIRECT3D9			pD3D_;				//Direct3Dインターフェイス
	static LPDIRECT3DDEVICE9	pD3DDevice_;		//Direct3Dデバイス
	static D3DPRESENT_PARAMETERS d3dpp_;			//デバイスのプレゼンテーションパラメータの作成
	static D3DDISPLAYMODE		d3ddm_;				//現在のディスプレイモードを取得	
	static LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	static LPDIRECT3DSURFACE9 renderTarget_;
	static LPDIRECT3DSURFACE9 depthStencil_;
	static LPDIRECT3DTEXTURE9 blurTexture1;				//ブラーテクスチャ１
	static LPDIRECT3DSURFACE9 blurSurface1;				//ブラーサーフェイス１
	static LPDIRECT3DTEXTURE9 blurTexture2;				//ブラーテクスチャ２
	static LPDIRECT3DSURFACE9 blurSurface2;				//ブラーサーフェイス２

public:
	// 構造体
	struct VERTEX
	{
		// 変換済み頂点
		FLOAT X, Y, Z, RHW;

		// ディフューズ色
		D3DCOLOR Diffuse;

		// テクスチャ座標
		FLOAT U0, V0;
		FLOAT U1, V1;
	};

	static BOOL Init(HWND hWnd, BOOL bWindow);
	static void Uninit();
	static void DrawBegin();
	static void DrawEnd();
	static void Clear();
	static void ClearZ();
	static void ResetRenderTarget();
	static LPDIRECT3DDEVICE9 GetDevice();
	static LPDIRECT3DTEXTURE9 GetBlurTexture1();
	static LPDIRECT3DTEXTURE9 GetBlurTexture2();
	static LPDIRECT3DSURFACE9 GetBlurSurface1();
	static LPDIRECT3DSURFACE9 GetBlurSurface2();
	static LPDIRECT3DSURFACE9 GetBackBuffer();
	static void ReplaceBlur();
	
	
};