#include "../Texture/Texture.h"
#include "Fade.h"
#include "../main.h"

constexpr float FadeSpeed = 3.0f;
constexpr float LOADWORD_WIDTH = 600.0f * (float)ScreenWidth / 1600.0f;
constexpr float LOADWORD_HEGIHT = 200.0f * (float)ScreenHeight / 900.0f;

Fade::Fade()
{
	tex_fade_ = new Texture(TextureManager::Tex_Fade);
	tex_fade_->Init();
	tex_fade_->SetColor(D3DCOLOR_RGBA(0, 0, 0, 0));
	tex_fade_->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	isFading_ = false;

	tex_loading_ = new Texture(TextureManager::Tex_NowLoading);
	tex_loading_->Init();
}

Fade::~Fade()
{
	if (tex_fade_ != NULL)
	{
		tex_fade_->Uninit();
		SAFE_DELETE(tex_fade_);
	}
	if (tex_loading_)
	{
		tex_loading_->Uninit();
		SAFE_DELETE(tex_loading_);
	}
}

void Fade::Init()
{
	tex_fade_->SetColor(D3DCOLOR_RGBA(0, 0, 0, 0));
	tex_fade_->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	isFading_ = false;

	tex_loading_->SetColor(D3DCOLOR_RGBA(255,255,255,0));
	tex_loading_->SetDrawSize(LOADWORD_WIDTH,LOADWORD_HEGIHT);
}

void Fade::Uninit()
{
	
}

void Fade::Update()
{
	tex_fade_->Update();
	if (fadeIn_)
	{
		fadeAlpha_+= FadeSpeed;
		isFading_ = true;
		if (fadeAlpha_ > 255.0f)
		{
			fadeAlpha_ = 255.0f;
			isFading_ = false;
		}
		tex_fade_->SetColor(D3DCOLOR_RGBA(0,0,0,(int)fadeAlpha_));
		tex_loading_->SetColor(D3DCOLOR_RGBA(255, 255, 255, (int)fadeAlpha_));
	}
	else
	{
		fadeAlpha_-= FadeSpeed;
		isFading_ = true;
		if (fadeAlpha_ < 0)
		{
			fadeAlpha_ = 0;
			isFading_ = false;
		}
		tex_fade_->SetColor(D3DCOLOR_RGBA(0, 0, 0, (int)fadeAlpha_));
		tex_loading_->SetColor(D3DCOLOR_RGBA(255, 255, 255, (int)fadeAlpha_));
	}
}

void Fade::BeginDraw()
{
}

void Fade::Draw()
{
	tex_fade_->Draw();
	tex_loading_->Draw();
}

void Fade::EndDraw()
{
}

void Fade::FadeIn()
{
	fadeIn_ = true;
	fadeAlpha_ = 0.0f;
}

void Fade::FadeOut()
{
	fadeIn_ = false;
	fadeAlpha_ = 255.0f;
}

bool Fade::IsFading()
{
	return isFading_;
}
