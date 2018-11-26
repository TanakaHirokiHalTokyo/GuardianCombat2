#include "Blur.h"
#include "../../DirectXRenderer.h"
#include "../../Texture/Texture.h"

Blur::Blur()
{
	blurTex_ = new Texture(TextureManager::Tex_Max);
	blurTex_->Init();
}

Blur::~Blur()
{
	if (blurTex_)
	{
		blurTex_->Uninit();
		delete blurTex_;
	}
}

void Blur::BeginDraw()
{
	if (useBlur_)
	{
		blurTex_->Update();

		LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
		LPDIRECT3DSURFACE9 surface1 = CRendererDirectX::GetBlurSurface1();
		pDevice->SetRenderTarget(0, surface1);
	}
}

void Blur::EndDraw()
{
	if (useBlur_)
	{
		LPDIRECT3DTEXTURE9 blurtexture = CRendererDirectX::GetBlurTexture2();
		float x = -(ScreenWidth* strength_ - ScreenWidth) / 2.0f;
		float y = -(ScreenHeight* strength_ - ScreenHeight) / 2.0f;

		blurTex_->SetPosition(x,y,0);
		blurTex_->SetTexture(blurtexture, ScreenWidth * strength_, ScreenHeight * strength_);
		blurTex_->SetDrawSize(ScreenWidth * strength_, ScreenHeight * strength_);
		blurTex_->SetTexcoordSize(ScreenWidth * strength_, ScreenHeight * strength_);
		blurTex_->SetColor(D3DCOLOR_RGBA(color_[0], color_[1], color_[2], alpha_));
		blurTex_->Draw();

		LPDIRECT3DSURFACE9 backbuffer = CRendererDirectX::GetBackBuffer();
		CRendererDirectX::ResetRenderTarget();

		LPDIRECT3DTEXTURE9 blurtexture1 = CRendererDirectX::GetBlurTexture1();
		blurTex_->SetPosition(0,0,0);
		blurTex_->SetTexture(blurtexture1, ScreenWidth, ScreenHeight);
		blurTex_->SetDrawSize(ScreenWidth, ScreenHeight);
		blurTex_->SetTexcoordSize(ScreenWidth, ScreenHeight);
		blurTex_->SetColor(D3DCOLOR_RGBA(color_[0], color_[1], color_[2], 255));
		blurTex_->Draw();

		CRendererDirectX::ReplaceBlur();
	}
}

void Blur::SetUseBlur(bool flag)
{
	useBlur_ = flag;
}

bool Blur::GetUseBlur()
{
	return useBlur_;
}

void Blur::SetAlpha(int value)
{
	if (value >= 0 || value <= 255)
	{
		alpha_ = value;
	}
}

void Blur::SetColor(int r, int g, int b)
{
	if (r >= 0 && r >= 255){	color_[0] = r;	}
	if (g >= 0 && g >= 255) { color_[1] = g; }
	if (b >= 0 && b >= 255) { color_[2] = b; }
}

void Blur::SetStrength(float value)
{
	if (value >= 1.0f)
	{
		strength_ = value;
	}
}
