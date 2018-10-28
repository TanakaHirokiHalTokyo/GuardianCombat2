#pragma once

#include "../Game/Object.h"

//Polygon
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

struct TEXTURE_3D
{
	char fileName[256];
	int width;
	int height;
	int texcoordX;
	int texcoordY;
};

struct VERTEX_2D
{
	D3DXVECTOR4 pos;
	D3DCOLOR color;
	D3DXVECTOR2 texcoord;
};

struct TEXPATTERN
{
	int patternNum;
	int patternW;
	int patternH;
	int tcx;
	int tcy;
};

class TextureManager
{
public:
	struct TexInfo
	{
		LPDIRECT3DTEXTURE9 pTex;
		int width;
		int height;
		int texcoordX;
		int texcoordY;
	};

	enum TextureList
	{
		Tex_0,
		Tex_Fade,
		Tex_Toon,
		Tex_Mission,
		Tex_Max,
	};

	static const TEXTURE_3D texInfo_[TextureList::Tex_Max];		//Textureの情報。パスとサイズ

	
	static void LoadAll();
	static TexInfo GetTexture(TextureList type);
	static void ReleaseAll();

private:
	static LPDIRECT3DTEXTURE9 pTexture_[Tex_Max];
};

class Texture : public Object
{
public:
	Texture(TextureManager::TextureList type);
	~Texture()override;

	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
	void SetRotation(float cx, float cy, float angle);
	void SetScale(float cx, float cy, float scaleX, float scaleY);
	void SetColor(D3DCOLOR color);
	D3DCOLOR GetColor();
	void SetDrawSize(float width, float height);
	D3DXVECTOR2 GetDrawSize() { return drawSize_; }
	void SetTexcoord(int tcx, int tcy);
	void SetTexture(LPDIRECT3DTEXTURE9 pTex, float texW, float texH);
	void SetTexcoordSize(float tcw, float tch);
	void SetAffine(bool Affine);

	TEXPATTERN ReturnTexPattern(int Count, int Frame, int texWNum, int texHNum, int texW, int texH);

protected:
	void CreateVertexAffine(float dw, float dh, int tcx, int tcy, int tcw, int tch);
	void CreateVertex(float dw, float dh, int tcx, int tcy, int tcw, int tch, int texW, int texH);

protected:
	LPDIRECT3DTEXTURE9 pTexture_;

	VERTEX_2D vertex;
	LPDIRECT3DVERTEXBUFFER9 g_pVertexBuffer = NULL;

	D3DCOLOR color_ = D3DCOLOR_RGBA(255,255,255,255);
	D3DXVECTOR2 texSize_;
	D3DXVECTOR2 texcoordSize_;
	D3DXVECTOR2 texcoord_;
	D3DXVECTOR2 drawSize_;
	bool		affine_;
	float		scaleAdd_ = 0.02f;
	float		angle_ = 0.0f;
	float		rotationOffsetX_ = 0.0f;
	float		rotationOffsetY_ = 0.0f;
	float		scaleOffsetX_ = 0.0f;
	float		scaleOffsetY_ = 0.0f;

protected:
	int priority_;
};
