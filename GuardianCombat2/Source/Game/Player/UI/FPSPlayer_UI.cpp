#include "FPSPlayer_UI.h"
#include "../../../Texture/Texture.h"
#include "../../Object.h"
#include "../../../main.h"
#include "../../GameManager/GameManager.h"
#include "../FPSPlayer.h"

const float RETICLE_SIZE = 50.0f;
//初期描画サイズ
const D3DXVECTOR2 InitDrawSize = D3DXVECTOR2((float)ScreenWidth / 2.0f, (float)ScreenHeight / 5.0f);

FPSPlayer_UI::FPSPlayer_UI()
{
	//レティクルテクスチャ生成
	reticle_ = Object::Create<Texture>(TextureManager::Tex_Reticle);
	//レティクルサイズ調整
	reticle_->SetDrawSize(RETICLE_SIZE,RETICLE_SIZE);
	//レティクル位置調整
	reticle_->SetPosition((float)ScreenWidth / 2.0f - RETICLE_SIZE / 2.0f,(float)ScreenHeight / 2.0f - RETICLE_SIZE / 2.0f,0.0f);
	//レティクルの色調整
	reticle_->SetColor(D3DCOLOR_RGBA(0,255,0,128));
	//レティクル可視化
	reticle_->SetVisible(true);

	//ライフバー作成
	lifeBar_ = Object::Create<Texture>(TextureManager::Tex_SliderBar);
	
}

FPSPlayer_UI::~FPSPlayer_UI()
{
}

void FPSPlayer_UI::Init()
{
	lifeBar_->SetDrawSize(InitDrawSize.x, InitDrawSize.y);
	lifeBar_->SetPosition(0, 0, 0);
	lifeBar_->SetColor(D3DCOLOR_RGBA(100, 255, 100, 200));
	lifeBar_->SetVisible(true);
}

void FPSPlayer_UI::Uninit()
{
}

void FPSPlayer_UI::Update()
{
	if (!player_){
		player_ = GameManager::GetPlayer();
	}

	if (player_){
		float rate = player_->GetLife() / PLAYER_DEFAULT_LIFE;
		lifeBar_->SetDrawSize(InitDrawSize.x * rate, InitDrawSize.y);
	}
}

void FPSPlayer_UI::Draw()
{
}
