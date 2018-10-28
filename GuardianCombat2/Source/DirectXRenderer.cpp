#include "main.h"
#include "DirectXRenderer.h"

// FVF
#define D3DFVF_VERTEX \
	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX2|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))


LPDIRECT3D9 CRendererDirectX::pD3D_ = NULL;
LPDIRECT3DDEVICE9 CRendererDirectX::pD3DDevice_ = NULL;
D3DPRESENT_PARAMETERS CRendererDirectX::d3dpp_;
D3DDISPLAYMODE	CRendererDirectX::d3ddm_;
LPDIRECT3DSURFACE9 CRendererDirectX::renderTarget_ = NULL;
LPDIRECT3DSURFACE9 CRendererDirectX::depthStencil_ = NULL;
LPDIRECT3DVERTEXBUFFER9 CRendererDirectX::VertexBuffer = NULL;

BOOL CRendererDirectX::Init(HWND hWnd, BOOL bWindow)
{

	//Direct3Dオブジェクトの作成
	pD3D_ = Direct3DCreate9(D3D_SDK_VERSION);
	if (pD3D_ == NULL)
	{
		return E_FAIL;
	}

	if (FAILED(pD3D_->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm_)))	//失敗したら真を返す
	{
		return E_FAIL;
	}

	ZeroMemory(&d3dpp_, sizeof(d3dpp_));								//メモリを初期化 d3dppの構造体を0で初期化
	d3dpp_.BackBufferWidth = (UINT)ScreenWidth;							//バックバッファの幅
	d3dpp_.BackBufferHeight = (UINT)ScreenHeight;							//				高さ
	d3dpp_.BackBufferFormat = d3ddm_.Format;							//ディスプレイモードのフォーマットを設定
	d3dpp_.BackBufferCount = 1;										//バックバッファの数
	d3dpp_.SwapEffect = D3DSWAPEFFECT_DISCARD;						//垂直同期を守るのか	DISCARDにしておけばいい
	d3dpp_.EnableAutoDepthStencil = TRUE;							//3次元のものをやるので必ずTRUE
	d3dpp_.AutoDepthStencilFormat = D3DFMT_D16;						//
	d3dpp_.Windowed = bWindow;										//TRUE:Windowモード		FALSE:FULLSCREEN
	d3dpp_.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		//FullScreen時のリフレッシュレート
	d3dpp_.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//垂直同期をみて更新

																	//デバイスオブジェクトの生成
																	//[デバイス作成制御0]<描画>と<頂点処理>を
	if (FAILED(pD3D_->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp_,
		&pD3DDevice_
	)))
	{
		//上記の設定が失敗したら
		//[デバイス作成制御]<描画>をハードウェア
		MessageBoxA(hWnd, "デバイス生成に失敗しました。", "ERROR", MB_OK);
		return false;
	};

	pD3DDevice_->GetRenderTarget(0,&renderTarget_);
	pD3DDevice_->GetDepthStencilSurface(&depthStencil_);

	// 頂点バッファの作成
	pD3DDevice_->CreateVertexBuffer(
		4 * sizeof(VERTEX), 0, D3DFVF_VERTEX,
		D3DPOOL_MANAGED, &VertexBuffer, NULL);
	return true;
}
void CRendererDirectX::Uninit()
{
	SAFE_RELEASE(pD3DDevice_);
	SAFE_RELEASE(pD3D_);
	SAFE_RELEASE(renderTarget_);
	SAFE_RELEASE(depthStencil_);
}
void CRendererDirectX::DrawBegin()
{
	pD3DDevice_->Clear(
		0,												//全画面クリア
		NULL,											//全画面クリア
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),			//クリアするﾌﾗｸﾞ TARGET:色　
		D3DCOLOR_RGBA(100, 100, 100, 255),				//クリアカラー TARGETをいれることで有効になる
		1.0f,											//ZBUFFERのクリア値
		0												//ステンシルのクリア数値	今はステンシルをﾌﾗｸﾞたててないので無視される
	);

	pD3DDevice_->BeginScene();

}
void CRendererDirectX::DrawEnd()
{
	pD3DDevice_->EndScene();							//Direct3Dによる描画の終了

	pD3DDevice_->Present(
		NULL,
		NULL,
		NULL,											//対象となるウィンドウハンドル
		NULL
	);
}
void CRendererDirectX::Clear()
{
	pD3DDevice_->Clear(
		0,												//全画面クリア
		NULL,											//全画面クリア
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),			//クリアするﾌﾗｸﾞ TARGET:色　
		D3DCOLOR_RGBA(255, 255, 255, 255),				//クリアカラー TARGETをいれることで有効になる
		1.0f,											//ZBUFFERのクリア値
		0												//ステンシルのクリア数値	今はステンシルをﾌﾗｸﾞたててないので無視される
	);
}
void CRendererDirectX::ClearZ()
{
	pD3DDevice_->Clear(
		0,												//全画面クリア
		NULL,											//全画面クリア
		D3DCLEAR_ZBUFFER,			//クリアするﾌﾗｸﾞ TARGET:色　
		D3DCOLOR_RGBA(255, 255, 255, 255),				//クリアカラー TARGETをいれることで有効になる
		1.0f,											//ZBUFFERのクリア値
		0												//ステンシルのクリア数値	今はステンシルをﾌﾗｸﾞたててないので無視される
	);
}
void CRendererDirectX::ResetRenderTarget()
{
	pD3DDevice_->SetRenderTarget(0,renderTarget_);
	pD3DDevice_->SetDepthStencilSurface(depthStencil_);
}
void CRendererDirectX::DrawQuad(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0, float v0, float u1, float v1, D3DCOLOR diffuse)
{
	// 頂点バッファをデバイスに設定
	LPDIRECT3DVERTEXBUFFER9 vb = VertexBuffer;
	pD3DDevice_->SetStreamSource(0, vb, 0, sizeof(VERTEX));
	pD3DDevice_->SetFVF(D3DFVF_VERTEX);

	// 頂点バッファのロック
	VERTEX* v;
	if (SUCCEEDED(
		vb->Lock(0, 4 * sizeof(VERTEX), (void**)&v, 0)
	)) {
		// 頂点座標の設定
		v[0].X = x0; v[0].Y = y0;
		v[1].X = x1; v[1].Y = y1;
		v[2].X = x2; v[2].Y = y2;
		v[3].X = x3; v[3].Y = y3;

		// テクスチャ座標の設定
		v[0].U0 = v[0].U1 = u0; v[0].V0 = v[0].V1 = v0;
		v[1].U0 = v[1].U1 = u1; v[1].V0 = v[1].V1 = v0;
		v[2].U0 = v[2].U1 = u0; v[2].V0 = v[2].V1 = v1;
		v[3].U0 = v[3].U1 = u1; v[3].V0 = v[3].V1 = v1;

		// Z, RHW, 頂点色の設定
		for (int i = 0; i < 4; i++) {
			v[i].Z = 0;
			v[i].RHW = 1;
			v[i].Diffuse = diffuse;
		}

		// 頂点バッファの解放
		vb->Unlock();

		// レンダリング条件の設定
		pD3DDevice_->SetRenderState(D3DRS_ZENABLE, FALSE);
		pD3DDevice_->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

		// プリミティブの描画
		pD3DDevice_->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		// レンダリング条件の復帰
		pD3DDevice_->SetRenderState(D3DRS_ZENABLE, TRUE);
		pD3DDevice_->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}
void CRendererDirectX::DrawQuad(ID3DXEffect * effect, float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3, float u0, float v0, float u1, float v1, D3DCOLOR diffuse)
{
	UINT p, np;
	effect->Begin(&np, 0);
	for (p = 0; p<np; p++) {
		effect->BeginPass(p);
		DrawQuad(x0, y0, x1, y1, x2, y2, x3, y3, u0, v0, u1, v1, diffuse);
		effect->EndPass();
	}
	effect->End();
}
LPDIRECT3DDEVICE9 CRendererDirectX::GetDevice()
{
	return pD3DDevice_;
}
