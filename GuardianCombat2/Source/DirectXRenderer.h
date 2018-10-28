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

	// 四角形の描画
	static void DrawQuad(
		float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	);
	static void DrawQuad(
		ID3DXEffect* effect,
		float x0, float y0, float x1, float y1,
		float x2, float y2, float x3, float y3,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	);
	static void DrawRect(
		ID3DXEffect* effect,
		float x0, float y0, float x1, float y1,
		float u0, float v0, float u1, float v1,
		D3DCOLOR diffuse
	) {
		DrawQuad(
			effect, x0, y0, x1, y0, x0, y1,
			x1, y1, u0, v0, u1, v1, diffuse);
	}
public:
	static LPDIRECT3DDEVICE9 GetDevice();
	static LPDIRECT3DSURFACE9 renderTarget_;
	static LPDIRECT3DSURFACE9 depthStencil_;
	
};