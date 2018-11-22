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
LPDIRECT3DTEXTURE9 CRendererDirectX::blurTexture1 = nullptr;				//ブラーテクスチャ１
LPDIRECT3DSURFACE9 CRendererDirectX::blurSurface1 = nullptr;				//ブラーサーフェイス１
LPDIRECT3DTEXTURE9 CRendererDirectX::blurTexture2 = nullptr;				//ブラーテクスチャ２
LPDIRECT3DSURFACE9 CRendererDirectX::blurSurface2 = nullptr;				//ブラーサーフェイス２

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
	D3DXCreateTexture(pD3DDevice_, ScreenWidth, ScreenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &blurTexture1);
	blurTexture1->GetSurfaceLevel(0, &blurSurface1);

	D3DXCreateTexture(pD3DDevice_, ScreenWidth, ScreenHeight, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &blurTexture2);
	blurTexture2->GetSurfaceLevel(0, &blurSurface2);

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
LPDIRECT3DDEVICE9 CRendererDirectX::GetDevice()
{
	return pD3DDevice_;
}

LPDIRECT3DTEXTURE9 CRendererDirectX::GetBlurTexture1()
{
	return blurTexture1;
}
LPDIRECT3DTEXTURE9 CRendererDirectX::GetBlurTexture2()
{
	return blurTexture2;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBlurSurface1()
{
	return blurSurface1;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBlurSurface2()
{
	return blurSurface2;
}
LPDIRECT3DSURFACE9 CRendererDirectX::GetBackBuffer()
{
	return renderTarget_;
}
void CRendererDirectX::ReplaceBlur()
{
	LPDIRECT3DTEXTURE9 texture;
	texture = blurTexture1;
	blurTexture1 = blurTexture2;
	blurTexture2 = texture;

	LPDIRECT3DSURFACE9 surface;
	surface = blurSurface1;
	blurSurface1 = blurSurface2;
	blurSurface2 = surface;
}