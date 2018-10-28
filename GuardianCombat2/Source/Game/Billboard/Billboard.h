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
		BilTex_Max
	};

	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	Billboard();
	~Billboard()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect,UINT pass);
	void EndDraw()override;
	void SetView(const D3DXMATRIX& view);
	D3DXMATRIX GetView();
	void SetTexture(TextureManager::TextureList texture);
	void SetColor(D3DCOLOR color) { this->color_ = color; }
	void SetTexcoord(int tcx, int tcy) { this->texcoord_.x = (float)tcx; this->texcoord_.y = (float)tcy; }
	void SetTexcoordSize(float tcw, float tch) { this->texcoordSize_.x = tcw; this->texcoordSize_.y = tch; }
	void SetStand(bool stand);
private:
	bool stand_ = false;
	LPDIRECT3DTEXTURE9 pTexture_ = NULL;
	D3DXMATRIX mtxView_ = {};
	D3DCOLOR color_ = D3DCOLOR_RGBA(255,255,255,255);
	D3DXVECTOR2 texcoord_ = D3DXVECTOR2(0, 0);
	D3DXVECTOR2 texSize_ = D3DXVECTOR2(1, 1);
	D3DXVECTOR2 texcoordSize_ = D3DXVECTOR2(1, 1);
	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer_ = NULL;
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer_ = NULL;
};