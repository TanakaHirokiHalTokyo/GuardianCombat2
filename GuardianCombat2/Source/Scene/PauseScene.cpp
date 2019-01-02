#include "PauseScene.h"
#include "../Texture/Texture.h"
#include "../Game/Object.h"
#include "../main.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"

const float PAUSE_WORD_SIZEX = (float)(ScreenWidth / 4.0f);		//ポーズ文字テクスチャ横幅
const float PAUSE_WORD_SIZEY = (float)(ScreenHeight / 4.0f);	//ポーズ文字テクスチャ縦幅

PauseScene::PauseScene()
{
	//ポーズ文字テクスチャ生成
	pauseWord_ = new Texture(TextureManager::Tex_PauseWord);

	//裏イメージ生成
	backImage_ = new Texture(TextureManager::Tex_Fade);
}

PauseScene::~PauseScene()
{
	if (pauseWord_)
	{
		pauseWord_->Uninit();
		delete pauseWord_;
		pauseWord_ = nullptr;
	}

	if (backImage_)
	{
		backImage_->Uninit();
		delete backImage_;
		backImage_ = nullptr;
	}
	
}

void PauseScene::Init()
{
	//ポーズ文字初期化
	pauseWord_->Init();
	pauseWord_->SetDrawSize(PAUSE_WORD_SIZEX,PAUSE_WORD_SIZEY);
	pauseWord_->SetPosition((float)(ScreenWidth - PAUSE_WORD_SIZEX) / 2.0f,(float)(ScreenHeight - PAUSE_WORD_SIZEY) / 2.0f,0.0f);	

	//裏イメージ初期化
	backImage_->Init();
	backImage_->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	backImage_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 128));
	backImage_->SetVisible(true);
}

void PauseScene::Uninit()
{
}

void PauseScene::Update()
{
	if (GetKeyboardTrigger(DIK_P)|| X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_START))
	{
		pausing_ = !pausing_;
	}

	if (pausing_)
	{
		backImage_->Update();
		pauseWord_->Update();
	}
}

void PauseScene::BeginDraw()
{
}

void PauseScene::Draw()
{
	if (pausing_)
	{
		backImage_->Draw();
		pauseWord_->Draw();
	}
}

void PauseScene::EndDraw()
{
}

void PauseScene::SetPause(bool flag)
{
	pausing_ = flag;
}

bool PauseScene::GetPause()
{
	return pausing_;
}
