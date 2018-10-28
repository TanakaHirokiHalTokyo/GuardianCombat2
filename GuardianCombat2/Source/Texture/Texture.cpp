
#include "Texture.h"
#include "../DirectXRenderer.h"

LPDIRECT3DTEXTURE9 TextureManager::pTexture_[TextureManager::Tex_Max];

//テクスチャ情報構造体
const TEXTURE_3D TextureManager::texInfo_[TextureManager::TextureList::Tex_Max]
{
	"resource/texture/0.png",						29, 31,29,31,
	"resource/texture/Dot.png",						1, 1,1,1,
	"resource/texture/ToonPaint.png",				256, 256,256,256,
	"resource/texture/Mission.png",					395, 70,395,70,
};
void TextureManager::LoadAll()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//テクスチャ―ステートの設定
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//WRAP...反復
	//CLAMP...引き伸ばし
	//MIRROR...反射
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//フィルタリング
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	for (unsigned int i = 0; i < Tex_Max; i++)
	{
		hr = D3DXCreateTextureFromFileA(pDevice, texInfo_[i].fileName, &pTexture_[i]);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, "テクスチャ読み込み失敗", "警告", MB_OK | MB_ICONHAND);
		}
	}
}
void TextureManager::ReleaseAll()
{
	for (unsigned int i = 0; i < Tex_Max; i++)
	{
		if (pTexture_[i] != nullptr)
		{
			pTexture_[i]->Release();
			pTexture_[i] = nullptr;
		}
	}
}
TextureManager::TexInfo TextureManager::GetTexture(TextureManager::TextureList type)
{
	TexInfo texture;

	texture.texcoordX = texInfo_[type].texcoordX;
	texture.texcoordY = texInfo_[type].texcoordY;
	texture.height = texInfo_[type].height;
	texture.width = texInfo_[type].width;
	texture.pTex = pTexture_[type];

	return texture;
}

Texture::Texture(TextureManager::TextureList type)
{
	Object::transform_.pos = D3DXVECTOR3(0,0,0);
	Object::transform_.scale = D3DXVECTOR3(1,1,1);

	TextureManager::TexInfo textureInfo = TextureManager::GetTexture(type);
	pTexture_ = textureInfo.pTex;
	this->texSize_ = D3DXVECTOR2((float)textureInfo.width, (float)textureInfo.height);
	this->texcoordSize_ = D3DXVECTOR2((float)textureInfo.texcoordX, (float)textureInfo.texcoordY);
	this->drawSize_ = D3DXVECTOR2((float)textureInfo.width, (float)textureInfo.height);
	this->texcoord_ = D3DXVECTOR2((float)0, (float)0);
	this->affine_ = false;
	this->color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
}
Texture::~Texture()
{
	
}
void Texture::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;

	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	//何バイト借りるか * ポリゴン数
		D3DUSAGE_WRITEONLY,		//使用用途 書き込みのみ　中身は保証されないので見に行けない
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,		//デバイスに管理させる
		&this->g_pVertexBuffer,		//メモの在り処
		NULL
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "頂点バッファが作られませんでした。", "ERROR", MB_OK);
	}
}
void Texture::Uninit()
{

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}
void Texture::Update()
{

}
void Texture::BeginDraw()
{
}
void Texture::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL) { return; }

	//レンダ―ステートの設定
	//αブレンドを行う
	//SRC...今から描くもの。つまりポリゴンにテクスチャが描画されるもの。
	//DEST...すでに描画されている画面のこと。
	//SRC_RGB * SRC_α + DEST_RGB * (1 - SRC_α)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//アルファテストをTRUEにする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX_2D);

	if (this->affine_ == true)
	{
		CreateVertexAffine(this->drawSize_.x, this->drawSize_.y, (int)this->texcoord_.x, (int)this->texcoord_.y, (int)this->texcoordSize_.x, (int)this->texcoordSize_.y);
	}
	else
	{
		CreateVertex(this->drawSize_.x, this->drawSize_.y, (int)this->texcoord_.x, (int)this->texcoord_.y, (int)this->texcoordSize_.x, (int)this->texcoordSize_.y, (int)this->texSize_.x, (int)this->texSize_.y);
	}
	//GPUとVertexBufferをパイプラインでつなぐ-+
	pDevice->SetStreamSource(
		0,
		g_pVertexBuffer,
		0,
		sizeof(VERTEX_2D)//隣の頂点までどれくらいの長さ
	);
	pDevice->SetTexture(0, this->pTexture_);
	//ポリゴンを描く
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,						//何バイト目から読み込むか
		2						//三角形
	);
	//アルファテストをFALSEにする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void Texture::EndDraw()
{
}
void Texture::CreateVertex(float dw, float dh, int tcx, int tcy, int tcw, int tch, int texW, int texH)
{
	VERTEX_2D* pV;

	g_pVertexBuffer->Lock(
		0,				//ここからロックしてほしい
		0,				//ここまでロック 0をいれると全部借りることができる
		(void**)&pV,	//疑似アドレス　ＣＰＵのメモリにいったんストアしてアンロックしたときにＶＲＡＭに転送される
		D3DLOCK_DISCARD	//ふらぐ　０でもいいがこのﾌﾗｸﾞをいれると効率がよくなる
	);

	int tw = texW;
	int th = texH;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	pV[0].pos = D3DXVECTOR4(transform_.pos.x + 0.0f - 0.5f, transform_.pos.y + 0.0f - 0.5f, 0.0f, 0.1f);
	pV[1].pos = D3DXVECTOR4(transform_.pos.x + dw - 0.5f, transform_.pos.y + 0.0f - 0.5f, 0.0f, 0.1f);
	pV[2].pos = D3DXVECTOR4(transform_.pos.x + 0.0f - 0.5f, transform_.pos.y + dh - 0.5f, 0.0f, 0.1f);
	pV[3].pos = D3DXVECTOR4(transform_.pos.x + dw - 0.5f, transform_.pos.y + dh - 0.5f, 0.0f, 0.1f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = this->color_;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();

}
void Texture::CreateVertexAffine(float dw, float dh, int tcx,int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);



	float x = (-dw - scaleOffsetX_) * scaleOffsetX_ + scaleOffsetX_ + dw;
	float x_w = (dw - scaleOffsetX_) * scaleOffsetX_ + scaleOffsetX_;
															 
	float y = (-dh - scaleOffsetY_) * scaleOffsetY_ + scaleOffsetY_ + dh;
	float y_h = (dh - scaleOffsetY_) * scaleOffsetY_ + scaleOffsetY_;

	float fRotationOffsetX = rotationOffsetX_;
	float fRotationOffsetY = rotationOffsetY_;

											
	x -= fRotationOffsetX;
	x_w -= fRotationOffsetX;

	y -= fRotationOffsetY;
	y_h -= fRotationOffsetY;

	transform_.pos.x += fRotationOffsetX - 0.5f;
	transform_.pos.y += fRotationOffsetY - 0.5f;

	int tw = (int)texSize_.x;
	int th = (int)texSize_.y;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

									
	pV[0].pos = D3DXVECTOR4(transform_.pos.x + x  * cosf(angle_) - y  * sinf(angle_), transform_.pos.y + x  * sinf(angle_) + y  * cosf(angle_), 0.0f, 0.1f);
	pV[1].pos = D3DXVECTOR4(transform_.pos.x + x_w * cosf(angle_) - y  * sinf(angle_), transform_.pos.y + x_w * sinf(angle_) + y  * cosf(angle_), 0.0f, 0.1f);
	pV[2].pos = D3DXVECTOR4(transform_.pos.x + x  * cosf(angle_) - y_h * sinf(angle_), transform_.pos.y + x  * sinf(angle_) + y_h * cosf(angle_), 0.0f, 0.1f);
	pV[3].pos = D3DXVECTOR4(transform_.pos.x + x_w * cosf(angle_) - y_h * sinf(angle_), transform_.pos.y + x_w * sinf(angle_) + y_h * cosf(angle_), 0.0f, 0.1f);

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color_;

	g_pVertexBuffer->Unlock();
}
//-------------------------------------------------------------
//	Rotation
//		cx cy : 回転させる際どこを基準に回転するか。
//				真ん中で回転させたい場合、元の画像サイズの1/2
//-------------------------------------------------------------
void Texture::SetRotation(float cx, float cy, float angle)
{
	this->angle_ = angle;
	this->rotationOffsetX_ = cx;
	this->rotationOffsetY_ = cy;
}
//------------------------------------------------------------
//	Scale
//		cx cy:どこから拡大させるか。
//		scaleX Y : 拡大率。元の画像サイズに対する拡大率。
//------------------------------------------------------------
void Texture::SetScale(float cx, float cy, float scaleX, float scaleY)
{
	this->scaleOffsetX_ = cx;
	this->scaleOffsetY_ = cy;
	Object::transform_.scale.x = scaleX;
	Object::transform_.scale.y = scaleY;
}
void Texture::SetColor(D3DCOLOR color)
{
	this->color_ = color;
}
D3DCOLOR Texture::GetColor()
{
	return color_;
}
TEXPATTERN Texture::ReturnTexPattern(int Count, int Frame, int texWNum, int texHNum, int texW, int texH)
{
	TEXPATTERN texpattern;
	texpattern.patternNum = (Count / Frame) % (texWNum * texHNum);
	texpattern.patternW = texpattern.patternNum % texWNum;
	texpattern.patternH = texpattern.patternNum / texWNum;
	texpattern.tcx = texpattern.patternW * texW;
	texpattern.tcy = texpattern.patternH * texH;

	return texpattern;
}
void Texture::SetDrawSize(float width, float height) {
	this->drawSize_.x = width;
	this->drawSize_.y = height;
}
void Texture::SetTexcoord(int tcx, int tcy) {
	this->texcoord_.x = (FLOAT)tcx;
	this->texcoord_.y = (FLOAT)tcy;
}
void Texture::SetTexture(LPDIRECT3DTEXTURE9 pTex, float texW, float texH)
{
	this->pTexture_ = pTex;
	this->texSize_.x = texW;
	this->texSize_.y = texH;
}
void Texture::SetTexcoordSize(float tcw, float tch) {
	this->texcoordSize_.x = tcw;
	this->texcoordSize_.y = tch;
}
void Texture::SetAffine(bool Affine) {
	this->affine_ = Affine;
}


