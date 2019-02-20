#include "PauseSelect.h"
#include "../PauseScene.h"	
#include "../../Texture/Texture.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../Imgui/ImguiManager.h"
#include "../../main.h"
#include "PauseSetting.h"
#include <fstream>
#include "../../Game/MouseState/MouseState.h"	
#include "../../Sound/Sound.h"

//���o�͂�����t�@�C����
const std::string FILENAME = "PauseSelectSettings";

PauseSelect::PauseSelect(PauseScene* scene)
{
	pauseScene_ = scene;

	LoadSettings(FILENAME);
	//�|�[�Y�ݒ萧�䐶��
	pauseSetting_ = new PauseSetting(this);
	pauseWord_ = new Texture(TextureManager::Tex_PauseWord);
	mouseWord_ =new Texture(TextureManager::Tex_MouseWord);
	controllerWord_ = new Texture(TextureManager::Tex_ControllerWord);
	backGame_ = new Texture(TextureManager::Tex_BackGame);
	backTitle_ = new Texture(TextureManager::Tex_BackTitle);
	selectBack_ = new Texture(TextureManager::Tex_SelectBack);

	selectingSE_ = new Sound(SoundManager::SELECTING_SE);
	selectSE_ = new Sound(SoundManager::SELECTOK_SE);
}

PauseSelect::~PauseSelect()
{
	if (pauseWord_)
	{
		pauseWord_->Uninit();
		SAFE_DELETE(pauseWord_);
	}
	if (mouseWord_)
	{
		mouseWord_->Uninit();
		SAFE_DELETE(mouseWord_);
	}
	if (controllerWord_)
	{
		controllerWord_->Uninit();
		SAFE_DELETE(controllerWord_);
	}
	SAFE_DELETE(selectBack_);
	SAFE_DELETE(backTitle_);
	SAFE_DELETE(backGame_);
	SAFE_DELETE(selectingSE_);
	SAFE_DELETE(selectSE_);
	SAFE_DELETE(pauseSetting_);
}

void PauseSelect::Init()
{
	pauseWord_->Init();
	mouseWord_->Init();
	controllerWord_->Init();
	pauseSetting_->Init();
	backGame_->Init();
	selectBack_->Init();

	//�����T�C�Y������
	pauseWord_->SetDrawSize(pauseSize_.x,pauseSize_.y);
	mouseWord_->SetDrawSize(mouseSize_.x, mouseSize_.y);
	controllerWord_->SetDrawSize(controllerSize_.x, controllerSize_.y);
	backGame_->SetDrawSize(backGameSize_.x, backGameSize_.y);
	backTitle_->SetDrawSize(backTitleSize_.x, backTitleSize_.y);
	selectBack_->SetDrawSize(selectBackSize_.x,selectBackSize_.y);
	//�����F������
	pauseWord_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	mouseWord_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	controllerWord_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	backGame_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	backTitle_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	selectBack_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	//�������W������
	pauseWord_->SetPosition(pausePos_.x, pausePos_.y, 0.0f);
	mouseWord_->SetPosition(mousePos_.x, mousePos_.y, 0.0f);
	controllerWord_->SetPosition(controllerPos_.x, controllerPos_.y, 0.0f);
	backGame_->SetPosition(backGamePos_.x, backGamePos_.y,0.0f);
	backTitle_->SetPosition(backTitlePos_.x, backTitlePos_.y, 0.0f);
}

void PauseSelect::Uninit()
{
	pauseWord_->Uninit();
	mouseWord_->Uninit();
	controllerWord_->Uninit();
	pauseSetting_->Uninit();
	selectBack_->Uninit();
}

void PauseSelect::Update()
{
	if (isSelecting_)		//�f�o�C�X�I�����
	{
		if (GameManager::GetEnableEdit())
		{
			ImGui::Begin(u8"�|�[�Y�I����ʕҏW");
			if (ImGui::Button(u8"�ۑ�")) SaveSettings(FILENAME);
			ImGui::ColorEdit3(u8"�ʏ펞�����F", stayColor_);
			ImGui::ColorEdit3(u8"�I���������F", selectingColor_);
			ImGui::Text(u8"�|�[�Y����");
			ImGui::DragFloat(u8"�|�[�YX���W", &this->pausePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�|�[�YY���W", &this->pausePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"�|�[�YX�T�C�Y", &this->pauseSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�|�[�YY�T�C�Y", &this->pauseSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"�}�E�X����");
			ImGui::DragFloat(u8"�}�E�XX���W", &this->mousePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�}�E�XY���W", &this->mousePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"�}�E�XX�T�C�Y", &this->mouseSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�}�E�XY�T�C�Y", &this->mouseSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"�R���g���[������");
			ImGui::DragFloat(u8"�R���g���[��X���W", &this->controllerPos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�R���g���[��Y���W", &this->controllerPos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"�R���g���[��X�T�C�Y", &this->controllerSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�R���g���[��Y�T�C�Y", &this->controllerSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"�Q�[���ɖ߂镶��");
			ImGui::DragFloat(u8"�Q�[���ɖ߂�X���W", &this->backGamePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�Q�[���ɖ߂�Y���W", &this->backGamePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"�Q�[���ɖ߂�X�T�C�Y", &this->backGameSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�Q�[���ɖ߂�Y�T�C�Y", &this->backGameSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"�^�C�g���ɖ߂镶��");
			ImGui::DragFloat(u8"�^�C�g���ɖ߂�X���W", &this->backTitlePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�^�C�g���ɖ߂�Y���W", &this->backTitlePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"�^�C�g���ɖ߂�X�T�C�Y", &this->backTitleSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"�^�C�g���ɖ߂�Y�T�C�Y", &this->backTitleSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"�I�𒆂̔w�i");
			ImGui::ColorEdit4(u8"�I�𒆂̔w�i�F",this->selectBackColor_);
			ImGui::DragFloat(u8"�I�𒆂̔w�iX�T�C�Y",&this->selectBackSize_.x,1.0f,0.0f,(float)ScreenWidth );
			ImGui::DragFloat(u8"�I�𒆂̔w�iY�T�C�Y", &this->selectBackSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::End();

			pauseWord_->SetPosition(pausePos_.x, pausePos_.y, 0.0f);
			mouseWord_->SetPosition(mousePos_.x, mousePos_.y, 0.0f);
			controllerWord_->SetPosition(controllerPos_.x, controllerPos_.y, 0.0f);
			backGame_->SetPosition(backGamePos_.x,backGamePos_.y,0.0f);
			backTitle_->SetPosition(backTitlePos_.x,backTitlePos_.y,0.0f);

			pauseWord_->SetDrawSize(pauseSize_.x, pauseSize_.y);
			mouseWord_->SetDrawSize(mouseSize_.x, mouseSize_.y);
			controllerWord_->SetDrawSize(controllerSize_.x, controllerSize_.y);
			backGame_->SetDrawSize(backGameSize_.x,backGameSize_.y);
			backTitle_->SetDrawSize(backTitleSize_.x,backTitleSize_.y);
			selectBack_->SetDrawSize(selectBackSize_.x,selectBackSize_.y);
		}

		pauseWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * stayColor_[0]), (int)(255.0f * stayColor_[1]), (int)(255.0f * stayColor_[2]), 255));
		mouseWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * stayColor_[0]), (int)(255.0f * stayColor_[1]), (int)(255.0f * stayColor_[2]), 255));
		controllerWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * stayColor_[0]), (int)(255.0f * stayColor_[1]), (int)(255.0f * stayColor_[2]), 255));
		backGame_->SetColor(D3DCOLOR_RGBA((int)(255.0f * stayColor_[0]), (int)(255.0f * stayColor_[1]), (int)(255.0f * stayColor_[2]), 255));
		backTitle_->SetColor(D3DCOLOR_RGBA((int)(255.0f * stayColor_[0]), (int)(255.0f * stayColor_[1]), (int)(255.0f * stayColor_[2]), 255));
		selectBack_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectBackColor_[0]), (int)(255.0f * selectBackColor_[1]), (int)(255.0f * selectBackColor_[2]), (int)(255.0f * selectBackColor_[3])));
		//----------------------------------------------------------------------------------------
		//		�N���b�N����		(�Q�[���ɖ߂�ƃ^�C�g���ɖ߂�͊֐����Ăяo���Ă̎g�p)
		if (isEnableClick_)
		{
			//�}�E�X�ł̓����蔻��
			DIMOUSESTATE MouseData = ReturnMouseMove();

			if (MouseState::IsHitMouseCursorBox(mouseWord_->GetPosition().x, mouseWord_->GetPosition().y,
				mouseWord_->GetDrawSize().x, mouseWord_->GetDrawSize().y))
			{
				if (selectingIndex_ != MOUSE)
				{
					selectingIndex_ = MOUSE;
					mouseWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
					selectingSE_->PlaySoundA();
				}
				if (MouseData.rgbButtons[0] == 0x80)
				{
					isSelecting_ = false;
					pauseSetting_->ChangeDevice((PauseSetting::DEVICE)selectingIndex_);
					pauseSetting_->SetCirclePosition();
					selectSE_->PlaySoundA();
				}
			}
			else if (MouseState::IsHitMouseCursorBox(controllerWord_->GetPosition().x, controllerWord_->GetPosition().y,
				controllerWord_->GetDrawSize().x, controllerWord_->GetDrawSize().y))
			{
				if (selectingIndex_ != CONTROLLER)
				{
					selectingIndex_ = CONTROLLER;
					controllerWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
					selectingSE_->PlaySoundA();
				}
				if (MouseData.rgbButtons[0] == 0x80)
				{
					isSelecting_ = false;
					pauseSetting_->ChangeDevice((PauseSetting::DEVICE)selectingIndex_);
					pauseSetting_->SetCirclePosition();
					selectSE_->PlaySoundA();
				}
			}
		}
		//---------------------------------------------------------------------------------------------------

		//------------------------------------------------------------------------------
		//		�L�[����ł̑I��		Xinput��Dinput�����Ή�

		//�R���g���[���[�X�e�B�b�N�X��
		float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);

		//��I��
		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) ||
			controllerLY >= 0.1f || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_UP))
		{
			if (selectingIndex_ > 0)		//��̕����Ɉړ�
			{
				selectingIndex_--;
				selectingSE_->PlaySoundA();
			}
		}
		//���I��
		if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) ||
			controllerLY <= -0.1f || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_DOWN))
		{
			if (selectingIndex_ < WORD_MAX - 1)		//���̕����Ɉړ�
			{
				selectingIndex_++;
				selectingSE_->PlaySoundA();
			}
		}
		//-------------------------------------------------------------------------------

		float offsetX = 0.0f;
		float offsetY = 0.0f;
		//���ݑI�𒆂̕����F�ύX
		switch (selectingIndex_)
		{
		case MOUSE:
			offsetX = (selectBack_->GetDrawSize().x - mouseWord_->GetDrawSize().x) / 2.0f;
			offsetY = (selectBack_->GetDrawSize().y - mouseWord_->GetDrawSize().y) / 2.0f;
			mouseWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
			selectBack_->SetPosition(mouseWord_->GetPosition().x - offsetX,mouseWord_->GetPosition().y - offsetY,0.0f);
			break;
		case CONTROLLER:
			offsetX = (selectBack_->GetDrawSize().x - controllerWord_->GetDrawSize().x) / 2.0f;
			offsetY = (selectBack_->GetDrawSize().y - controllerWord_->GetDrawSize().y) / 2.0f;
			controllerWord_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
			selectBack_->SetPosition(controllerWord_->GetPosition().x - offsetX, controllerWord_->GetPosition().y - offsetY, 0.0f);
			break;
		case BACKGAME:
			offsetX = (selectBack_->GetDrawSize().x - backGame_->GetDrawSize().x) / 2.0f;
			offsetY = (selectBack_->GetDrawSize().y - backGame_->GetDrawSize().y) / 2.0f;
			backGame_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
			selectBack_->SetPosition(backGame_->GetPosition().x - offsetX, backGame_->GetPosition().y - offsetY, 0.0f);
			break;
		case BACKTITLE:
			offsetX = (selectBack_->GetDrawSize().x - backTitle_->GetDrawSize().x) / 2.0f;
			offsetY = (selectBack_->GetDrawSize().y - backTitle_->GetDrawSize().y) / 2.0f;
			backTitle_->SetColor(D3DCOLOR_RGBA((int)(255.0f * selectingColor_[0]), (int)(255.0f * selectingColor_[1]), (int)(255.0f * selectingColor_[2]), 255));
			selectBack_->SetPosition(backTitle_->GetPosition().x - offsetX, backTitle_->GetPosition().y - offsetY, 0.0f);
			break;
		default:
			break;
		}
		
		//���肵���ۂ̏���
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
		{
			//�}�E�X�E�R���g���[���̏ꍇ
			switch (selectingIndex_)
			{
			case MOUSE:
				isSelecting_ = false;
				pauseSetting_->ChangeDevice((PauseSetting::DEVICE)MOUSE);		//�f�o�C�X�ؑ�
				pauseSetting_->SetCirclePosition();				//�{�^���̈ʒu����
				break;
			case CONTROLLER:
				isSelecting_ = false;
				pauseSetting_->ChangeDevice((PauseSetting::DEVICE)CONTROLLER);		//�f�o�C�X�ؑ�
				pauseSetting_->SetCirclePosition();				//�{�^���̈ʒu����
				break;
			case BACKGAME:
				ClosePause();
				pauseScene_->SetPause(false);
				break;
			case BACKTITLE:
				GameManager::SetReturnTitle(true);
				break;
			default:
				break;
			}
			selectSE_->PlaySoundA();
		}
		if(pauseWord_)		pauseWord_->Update();
		if (mouseWord_)		mouseWord_->Update();
		if (controllerWord_) controllerWord_->Update();
		if (backGame_)		backGame_->Update();
		if (backTitle_)			backTitle_->Update();
	}
	else		//�f�o�C�X�ݒ蒆�̏���
	{
		pauseSetting_->Update();

		//�f�o�C�X�I����ʂɖ߂�
		if (GetKeyboardTrigger(DIK_B))
		{
			isSelecting_ = true;
			selectSE_->PlaySoundA();
			pauseSetting_->SaveSensitivity(PauseSetting::SENCITIVITY_FILENAME);
		}
		
	}
}

void PauseSelect::Draw()
{
	if (isSelecting_)
	{
		if (selectBack_)		selectBack_->Draw();
		if (pauseWord_)		pauseWord_->Draw();
		if (mouseWord_)		mouseWord_->Draw();
		if (controllerWord_) controllerWord_->Draw();
		if (backGame_)		backGame_->Draw();
		if (backTitle_)			backTitle_->Draw();
	}
	else
	{
		pauseSetting_->Draw();
	}
}

void PauseSelect::ClosePause()
{
	isSelecting_ = true;
}

bool PauseSelect::IsHitMouseBackGameWord()
{
	if (MouseState::IsHitMouseCursorBox(backGame_->GetPosition().x, backGame_->GetPosition().y,
		backGame_->GetDrawSize().x, backGame_->GetDrawSize().y))
	{
		selectingIndex_ = BACKGAME;
		return true;
	}
	return false;
}

bool PauseSelect::IsHitMouseBackTitleWord()
{
	if (MouseState::IsHitMouseCursorBox(backTitle_->GetPosition().x, backTitle_->GetPosition().y,
		backTitle_->GetDrawSize().x, backTitle_->GetDrawSize().y))
	{
		selectingIndex_ = BACKTITLE;
		return true;
	}
	return false;
}

void PauseSelect::SaveSettings(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::out);
	file.write((const char*)&this->stayColor_,sizeof(this->stayColor_));
	file.write((const char*)&this->selectingColor_,sizeof(this->selectingColor_));
	file.write((const char*)&this->pausePos_,sizeof(this->pausePos_));
	file.write((const char*)&this->pauseSize_,sizeof(this->pauseSize_));
	file.write((const char*)&this->mousePos_, sizeof(this->mousePos_));
	file.write((const char*)&this->mouseSize_, sizeof(this->mouseSize_));
	file.write((const char*)&this->controllerPos_, sizeof(this->controllerPos_));
	file.write((const char*)&this->controllerSize_, sizeof(this->controllerSize_));
	file.write((const char*)&this->backGamePos_, sizeof(this->backGamePos_));
	file.write((const char*)&this->backGameSize_, sizeof(this->backGameSize_));
	file.write((const char*)&this->backTitlePos_, sizeof(this->backTitlePos_));
	file.write((const char*)&this->backTitleSize_, sizeof(this->backTitleSize_));
	file.write((const char*)&this->selectBackSize_, sizeof(this->selectBackSize_));
	file.write((const char*)&this->selectBackColor_, sizeof(this->selectBackColor_));
	file.close();
}

void PauseSelect::LoadSettings(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�|�[�Y�I����ʃf�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->stayColor_, sizeof(this->stayColor_));
		file.read((char*)&this->selectingColor_, sizeof(this->selectingColor_));
		file.read((char*)&this->pausePos_, sizeof(this->pausePos_));
		file.read((char*)&this->pauseSize_, sizeof(this->pauseSize_));
		file.read((char*)&this->mousePos_, sizeof(this->mousePos_));
		file.read((char*)&this->mouseSize_, sizeof(this->mouseSize_));
		file.read((char*)&this->controllerPos_, sizeof(this->controllerPos_));
		file.read((char*)&this->controllerSize_, sizeof(this->controllerSize_));
		file.read((char*)&this->backGamePos_, sizeof(this->backGamePos_));
		file.read((char*)&this->backGameSize_, sizeof(this->backGameSize_));
		file.read((char*)&this->backTitlePos_, sizeof(this->backTitlePos_));
		file.read((char*)&this->backTitleSize_, sizeof(this->backTitleSize_));
		file.read((char*)&this->selectBackSize_, sizeof(this->selectBackSize_));
		file.read((char*)&this->selectBackColor_, sizeof(this->selectBackColor_));
	}
	file.close();
}
