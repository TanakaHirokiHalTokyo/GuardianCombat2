#include "Effect.h"
#include <d3dx9.h>
#include <d3d9.h>
#include "../../DirectXRenderer.h"
#include "../../Camera/Camera.h"

LPDIRECT3DTEXTURE9 EffectManager::texture_ = nullptr;
LPDIRECT3DVERTEXBUFFER9 EffectManager::vertexBuffer_ = nullptr;
LPDIRECT3DINDEXBUFFER9 EffectManager::indexBuffer_ = nullptr;
EffectManager::VERTEX3D* EffectManager::vertex_ = nullptr;

void EffectManager::Init()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	WORD* pIndex;
	HRESULT hr;

	//エフェクトテクスチャ読み込み
	const char filename[256] = "resource/texture/Effect_100.png";
	hr = D3DXCreateTextureFromFileA(pDevice, filename,&texture_);
	if (FAILED(hr))
	{
		MessageBoxA(NULL,"エフェクト読み込み失敗","警告",MB_OK | MB_ICONHAND);
	}

	//VertexBuffer作成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX3D,
		D3DPOOL_MANAGED,
		&vertexBuffer_,
		NULL
	);

	//Vertex情報登録
	vertexBuffer_->Lock(
		0,
		0,
		(void**)&vertex_,
		D3DLOCK_DISCARD
	);
	vertex_[0] = { D3DXVECTOR3(-0.5f,0.5f,0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(0.0f,0.0f) };
	vertex_[1] = { D3DXVECTOR3(0.5f, 0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(1.0f,0.0f) };
	vertex_[2] = { D3DXVECTOR3(0.5f, -0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(1.0f,1.0f) };
	vertex_[3] = { D3DXVECTOR3(-0.5f, -0.5f, 0.0f),D3DXVECTOR3(0,0,1.0f),D3DCOLOR_RGBA(255,255,255,150),D3DXVECTOR2(0.0f,1.0f) };

	vertexBuffer_->Unlock();

	//Indexバッファ作成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * 6,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&indexBuffer_,
		NULL
	);

	//Index情報登録
	indexBuffer_->Lock(0,0,(void**)&pIndex,D3DLOCK_DISCARD);

	pIndex[0] = 0;
	pIndex[1] = 1;
	pIndex[2] = 2;
	pIndex[3] = 0;
	pIndex[4] = 2;
	pIndex[5] = 3;

	indexBuffer_->Unlock();


}

void EffectManager::Uninit()
{
	if (texture_)
	{
		texture_->Release();
		texture_ = nullptr;
	}
	if (indexBuffer_)
	{
		indexBuffer_->Release();
		indexBuffer_ = nullptr;
	}
	if (vertexBuffer_)
	{
		vertexBuffer_->Release();
		vertexBuffer_ = nullptr;
	}
}

LPDIRECT3DTEXTURE9 EffectManager::GetTexture()
{
	return texture_;
}

LPDIRECT3DVERTEXBUFFER9& EffectManager::GetVertexBuffer()
{
	return vertexBuffer_;
}

LPDIRECT3DINDEXBUFFER9& EffectManager::GetIndexBuffer()
{
	return indexBuffer_;
}

EffectManager::VERTEX3D & EffectManager::GetVertex()
{
	return *vertex_;
}

AdditionEffect::AdditionEffect()
{
	limit_ = 150;
	for (int i = 0; i < EFFECTNUM; i++)
	{
		color_[i].r = 1.0f;
		color_[i].g = 1.0f;
		color_[i].b = 1.0f;
		color_[i].a = 1.0f;
		transform_[i].scale = D3DXVECTOR3(1, 1, 1);
		transform_[i].pos = D3DXVECTOR3(0,0,0);
		count_[i] = 0;
		use_[i] = false;
	}
}

AdditionEffect::~AdditionEffect()
{
	
}

void AdditionEffect::Init()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		count_[i] = 0;
		use_[i] = false;
		color_[i].a = 1.0f;
		transform_[i].pos = D3DXVECTOR3(0,0,0);
	}
}

void AdditionEffect::Uninit()
{
	limit_ = 150;
	for (int i = 0; i < EFFECTNUM; i++)
	{
		count_[i] = 0;
		use_[i] = false;
		color_[i].a = 0.0f;
		transform_[i].pos = D3DXVECTOR3(0, 0, 0);
	}
}

void AdditionEffect::Update()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (use_[i])
		{
			//割合計算
			float ratio = (float)(count_[i] / limit_);

			//α値更新
			color_[i].a = 1.0f - ratio;

			if (ratio >= 1.0f)
			{
				use_[i] = false;
			}

			//カウンタ更新
			count_[i]++;
		}
	}
}

void AdditionEffect::Draw()
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (use_[i])
		{
			//デバイス取得
			LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();

			//vertex情報取得
			EffectManager::VERTEX3D* vertex = &EffectManager::GetVertex();

			EffectManager::GetVertexBuffer()->Lock(
				0, 0, (void**)&vertex, D3DLOCK_DISCARD);

			//カラー再セット
			vertex[0].color = (D3DCOLOR)color_[i];
			vertex[1].color = (D3DCOLOR)color_[i];
			vertex[2].color = (D3DCOLOR)color_[i];
			vertex[3].color = (D3DCOLOR)color_[i];

			EffectManager::GetVertexBuffer()->Unlock();

			//カメラ情報取得
			CAMERA_INFO cam_info = Camera::GetCameraInfo();
			D3DXMATRIX view = cam_info.view;

			//逆行列に変換
			D3DXMatrixTranspose(&view, &view);

			view._14 = 0.0f;
			view._24 = 0.0f;
			view._34 = 0.0f;

			D3DXMATRIX mtxTrans, mtxScale, mtxWorld;

			//移動・拡大行列作成
			D3DXMatrixTranslation(&mtxTrans, transform_[i].pos.x, transform_[i].pos.y, transform_[i].pos.z);
			D3DXMatrixScaling(&mtxScale, transform_[i].scale.x, transform_[i].scale.y, transform_[i].scale.z);

			mtxWorld = view;
			mtxWorld *= mtxScale;
			mtxWorld *= mtxTrans;

			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			//FVFの設定
			pDevice->SetFVF(FVF_VERTEX3D);
			//ライトオフにする
			pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			//テクスチャセット
			pDevice->SetTexture(0, EffectManager::GetTexture());
			//各種行列設定
			pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

			// GPUとVertexBufferをパイプラインでつなぐ - +
			pDevice->SetStreamSource(
				0,										//パイプラインのセット番号
				EffectManager::GetVertexBuffer(),		//バッファ
				0,										//どこから書き込むか
				sizeof(EffectManager::VERTEX3D)			//隣の頂点までどれくらいの長さ
			);

			// デバイスにインデックスセット
			pDevice->SetIndices(
				EffectManager::GetIndexBuffer()			//バッファ
			);

			//次からバッファ使いたくないときが来たときバッファをNULLにすれば使わなくなる。

			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0,
				0,
				4,			//頂点数
				0,
				2			//プリミティブ数
			);


			//ライトオンにする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			////通常のαブレンド（半透明処理）に戻す
			pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		}
	}

	
}

void AdditionEffect::CreateEffect(D3DXVECTOR3 pos)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		if (!use_[i])
		{
			use_[i] = true;
			transform_[i].pos = pos;
			break;
		}
	}
}
void AdditionEffect::SetLimit(int limit)
{
	limit_ = limit;
}

void AdditionEffect::SetColor(float r,float g,float b)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		color_[i].r = r;
		color_[i].g = g;
		color_[i].b = b;
	}
}

void AdditionEffect::SetScale(float size)
{
	for (int i = 0; i < EFFECTNUM; i++)
	{
		transform_[i].scale = D3DXVECTOR3(size, size, size);
	}
}
