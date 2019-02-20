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

//入出力をするファイル名
const std::string FILENAME = "PauseSelectSettings";

PauseSelect::PauseSelect(PauseScene* scene)
{
	pauseScene_ = scene;

	LoadSettings(FILENAME);
	//ポーズ設定制御生成
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

	//文字サイズ初期化
	pauseWord_->SetDrawSize(pauseSize_.x,pauseSize_.y);
	mouseWord_->SetDrawSize(mouseSize_.x, mouseSize_.y);
	controllerWord_->SetDrawSize(controllerSize_.x, controllerSize_.y);
	backGame_->SetDrawSize(backGameSize_.x, backGameSize_.y);
	backTitle_->SetDrawSize(backTitleSize_.x, backTitleSize_.y);
	selectBack_->SetDrawSize(selectBackSize_.x,selectBackSize_.y);
	//文字色初期化
	pauseWord_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	mouseWord_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	controllerWord_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	backGame_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	backTitle_->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	selectBack_->SetColor(D3DCOLOR_RGBA(255,255,255,255));
	//文字座標初期化
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
	if (isSelecting_)		//デバイス選択画面
	{
		if (GameManager::GetEnableEdit())
		{
			ImGui::Begin(u8"ポーズ選択画面編集");
			if (ImGui::Button(u8"保存")) SaveSettings(FILENAME);
			ImGui::ColorEdit3(u8"通常時文字色", stayColor_);
			ImGui::ColorEdit3(u8"選択時文字色", selectingColor_);
			ImGui::Text(u8"ポーズ文字");
			ImGui::DragFloat(u8"ポーズX座標", &this->pausePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"ポーズY座標", &this->pausePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"ポーズXサイズ", &this->pauseSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"ポーズYサイズ", &this->pauseSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"マウス文字");
			ImGui::DragFloat(u8"マウスX座標", &this->mousePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"マウスY座標", &this->mousePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"マウスXサイズ", &this->mouseSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"マウスYサイズ", &this->mouseSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"コントローラ文字");
			ImGui::DragFloat(u8"コントローラX座標", &this->controllerPos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"コントローラY座標", &this->controllerPos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"コントローラXサイズ", &this->controllerSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"コントローラYサイズ", &this->controllerSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"ゲームに戻る文字");
			ImGui::DragFloat(u8"ゲームに戻るX座標", &this->backGamePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"ゲームに戻るY座標", &this->backGamePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"ゲームに戻るXサイズ", &this->backGameSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"ゲームに戻るYサイズ", &this->backGameSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"タイトルに戻る文字");
			ImGui::DragFloat(u8"タイトルに戻るX座標", &this->backTitlePos_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"タイトルに戻るY座標", &this->backTitlePos_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::DragFloat(u8"タイトルに戻るXサイズ", &this->backTitleSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
			ImGui::DragFloat(u8"タイトルに戻るYサイズ", &this->backTitleSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
			ImGui::Text(u8"選択中の背景");
			ImGui::ColorEdit4(u8"選択中の背景色",this->selectBackColor_);
			ImGui::DragFloat(u8"選択中の背景Xサイズ",&this->selectBackSize_.x,1.0f,0.0f,(float)ScreenWidth );
			ImGui::DragFloat(u8"選択中の背景Yサイズ", &this->selectBackSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
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
		//		クリック操作		(ゲームに戻るとタイトルに戻るは関数を呼び出しての使用)
		if (isEnableClick_)
		{
			//マウスでの当たり判定
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
		//		キー操作での選択		XinputとDinput両方対応

		//コントローラースティック傾き
		float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);

		//上選択
		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) ||
			controllerLY >= 0.1f || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_UP))
		{
			if (selectingIndex_ > 0)		//上の文字に移動
			{
				selectingIndex_--;
				selectingSE_->PlaySoundA();
			}
		}
		//下選択
		if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) ||
			controllerLY <= -0.1f || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_DOWN))
		{
			if (selectingIndex_ < WORD_MAX - 1)		//下の文字に移動
			{
				selectingIndex_++;
				selectingSE_->PlaySoundA();
			}
		}
		//-------------------------------------------------------------------------------

		float offsetX = 0.0f;
		float offsetY = 0.0f;
		//現在選択中の文字色変更
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
		
		//決定した際の処理
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
		{
			//マウス・コントローラの場合
			switch (selectingIndex_)
			{
			case MOUSE:
				isSelecting_ = false;
				pauseSetting_->ChangeDevice((PauseSetting::DEVICE)MOUSE);		//デバイス切替
				pauseSetting_->SetCirclePosition();				//ボタンの位置決定
				break;
			case CONTROLLER:
				isSelecting_ = false;
				pauseSetting_->ChangeDevice((PauseSetting::DEVICE)CONTROLLER);		//デバイス切替
				pauseSetting_->SetCirclePosition();				//ボタンの位置決定
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
	else		//デバイス設定中の処理
	{
		pauseSetting_->Update();

		//デバイス選択画面に戻る
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
		MessageBoxA(NULL, "ポーズ選択画面データを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
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
