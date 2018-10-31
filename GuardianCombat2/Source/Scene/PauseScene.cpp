#include "PauseScene.h"
#include "../Texture/Texture.h"
#include "../Game/Object.h"
#include "../main.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"

const float PAUSE_WORD_SIZEX = (float)(ScreenWidth / 4.0f);		//�|�[�Y�����e�N�X�`������
const float PAUSE_WORD_SIZEY = (float)(ScreenHeight / 4.0f);	//�|�[�Y�����e�N�X�`���c��

PauseScene::PauseScene()
{
	//�|�[�Y�����e�N�X�`������
	pauseWord_ = new Texture(TextureManager::Tex_PauseWord);
}

PauseScene::~PauseScene()
{
	if (pauseWord_)
	{
		pauseWord_->Uninit();
		delete pauseWord_;
		pauseWord_ = nullptr;
	}
	
}

void PauseScene::Init()
{
	pauseWord_->Init();
	pauseWord_->SetDrawSize(PAUSE_WORD_SIZEX,PAUSE_WORD_SIZEY);
	pauseWord_->SetPosition((float)(ScreenWidth - PAUSE_WORD_SIZEX) / 2.0f,(float)(ScreenHeight - PAUSE_WORD_SIZEY) / 2.0f,0.0f);	
}

void PauseScene::Uninit()
{
}

void PauseScene::Update()
{
	if (GetKeyboardTrigger(DIK_P))
	{
		pausing_ = !pausing_;
	}

	if (pausing_)
	{
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
