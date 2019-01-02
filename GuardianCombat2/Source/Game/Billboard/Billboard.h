#pragma once

#include "../Object.h"
#include "../../Texture/Texture.h"

class Billboard : public Object
{
public :
	enum Billboard_Tex
	{
		BilTex_Turret,					
		BilTex_Difficulity,
		BilTex_SimpleBullet,
		BilTex_Max
	};

	struct VERTEX3D									//頂点構造体
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	Billboard();																		//コンストラクタ
	~Billboard()override;														//デストラクタ
	void Init()override;															//初期化処理
	void Uninit()override;														//終了処理
	void Update()override;													//更新処理
	void BeginDraw()override;												//事前描画処理
	void Draw()override;														//描画処理
	void Draw(LPD3DXEFFECT effect,UINT pass);				//描画処理シェーダー使用時
	void EndDraw()override;													//描画終了処理
	void SetView(const D3DXMATRIX& view);						//ビュー行列設定
	D3DXMATRIX GetView();												//ビュー行列取得
	void SetTexture(TextureManager::TextureList texture);		//テクスチャ取得
	void SetColor(D3DCOLOR color) { this->color_ = color; }	//カラー設定
	void SetTexcoord(int tcx, int tcy) { this->texcoord_.x = (float)tcx; this->texcoord_.y = (float)tcy; }					//テクスチャ座標設定
	void SetTexcoordSize(float tcw, float tch) { this->texcoordSize_.x = tcw; this->texcoordSize_.y = tch; }		//テクスチャ1アニメーションの大きさ設定
	void SetStand(bool stand);												//Pitchをさせるのか
private:
	bool stand_ = false;																						//Pitch制御フラグ　
	LPDIRECT3DTEXTURE9 pTexture_ = NULL;												//テクスチャポインタ
	D3DXMATRIX mtxView_ = {};																		//ビュー行列
	D3DCOLOR color_ = D3DCOLOR_RGBA(255,255,255,255);						//色情報
	D3DXVECTOR2 texcoord_ = D3DXVECTOR2(0, 0);									//テクスチャ座標
	D3DXVECTOR2 texSize_ = D3DXVECTOR2(1, 1);										//テクスチャサイズ
	D3DXVECTOR2 texcoordSize_ = D3DXVECTOR2(1, 1);								//テクスチャ１アニメーションの大きさ
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer_ = NULL;							//頂点バッファ
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer_ = NULL;								//インデックスバッファ
};