#include "PauseScene.h"
#include "../Texture/Texture.h"
#include "../Game/Object.h"
#include "../main.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"
#include "Pause\PauseSelect.h"
#include "../Game/GameManager/GameManager.h"
#include "TitleScene.h"
#include "Fade.h"
#include "Pause\PauseSetting.h"

PauseScene::PauseScene()
{
	//���C���[�W����
	backImage_ = new Texture(TextureManager::Tex_Fade);
	//�|�[�Y�I�𐧌䐶��
	pauseSelect_ = new PauseSelect();
}

PauseScene::~PauseScene()
{
	if (backImage_)
	{
		backImage_->Uninit();
		delete backImage_;
		backImage_ = nullptr;
	}
	if (pauseSelect_)
	{
		pauseSelect_->Uninit();
		delete pauseSelect_;
		pauseSelect_ = nullptr;
	}
}

void PauseScene::Init()
{
	//���C���[�W������
	backImage_->Init();
	backImage_->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	backImage_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 128));
	backImage_->SetVisible(true);
	
	pauseSelect_->Init();
}

void PauseScene::Uninit()
{
	pauseSelect_->Uninit();
}

void PauseScene::Update()
{
	if (GetKeyboardTrigger(DIK_P)|| X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_START))
	{
		pauseSelect_->ClosePause();
		pausing_ = !pausing_;
	}

	//�|�[�Y������
	if (pausing_)
	{
		backImage_->Update();
		pauseSelect_->Update();

		//�}�E�X�ł�UI�Ƃ̓����蔻��
		DIMOUSESTATE MouseData = ReturnMouseMove();
		if (MouseData.rgbButtons[0] == 0x80)
		{
			if (pauseSelect_->IsHitMouseBackGameWord())
			{
				pauseSelect_->ClosePause();
				pausing_ = false;
			}
			if (pauseSelect_->IsHitMouseBackTitleWord())
			{
				GameManager::SetReturnTitle(true);
			}
		}
	}	
	pauseSelect_->SetEnableClick(isEnableClick_);
}

void PauseScene::BeginDraw()
{
}

void PauseScene::Draw()
{
	if (pausing_)
	{
		backImage_->Draw();
		pauseSelect_->Draw();
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
