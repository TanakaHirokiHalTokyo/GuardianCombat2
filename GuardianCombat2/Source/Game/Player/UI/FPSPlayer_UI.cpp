#include "FPSPlayer_UI.h"
#include "../../../Texture/Texture.h"
#include "../../Object.h"
#include "../../../main.h"

FPSPlayer_UI::FPSPlayer_UI()
{
	//���e�B�N���e�N�X�`������
	reticle_ = Object::Create<Texture>(TextureManager::Tex_Reticle);
	//���e�B�N���T�C�Y����
	reticle_->SetDrawSize(RETICLE_SIZE,RETICLE_SIZE);
	//���e�B�N���ʒu����
	reticle_->SetPosition((float)ScreenWidth / 2.0f - RETICLE_SIZE / 2.0f,(float)ScreenHeight / 2.0f - RETICLE_SIZE / 2.0f,0.0f);
	//���e�B�N���̐F����
	reticle_->SetColor(D3DCOLOR_RGBA(0,255,0,128));
	//���e�B�N������
	reticle_->SetVisible(true);
}

FPSPlayer_UI::~FPSPlayer_UI()
{
}

void FPSPlayer_UI::Init()
{
}

void FPSPlayer_UI::Uninit()
{
}

void FPSPlayer_UI::Update()
{
}

void FPSPlayer_UI::Draw()
{
}
