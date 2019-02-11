#include "SelectSkillScene.h"
#include "../Game/Object.h"
#include "SelectSkill\SelectSkillUI.h"
#include "../Game/GameManager/GameManager.h"
#include "Fade.h"
#include "SelectSkill\SelectSkillEffect.h"
#include "../XInput/xcontroller.h"
#include "../DInput/DirectInput.h"
#include "../Imgui/ImguiManager.h"
#include "GameScene.h"

constexpr size_t CONTROLLER_COUNTER_VALUE = 15;	//コントローラースティック制御カウンタ

SelectSkillScene::SelectSkillScene()
{
	//FadeOut
	sceneChange_ = false;
	fade_ = GameManager::GetFade();
	fade_->FadeOut();
	uiSystem_ = new SelectSkillUI(this);
	efSystem_ = new SelectSkillEffect(this);
}

SelectSkillScene::~SelectSkillScene()
{
	if (uiSystem_)
	{
		delete uiSystem_;
		uiSystem_ = nullptr;
	}
	if (efSystem_)
	{
		delete efSystem_;
		efSystem_ = nullptr;
	}
}

void SelectSkillScene::Init()
{
	if (uiSystem_)	uiSystem_->Init();
	if (efSystem_) efSystem_->Init();
	Object::InitAll();
}

void SelectSkillScene::Uninit()
{
	if (uiSystem_)	uiSystem_->Uninit();
	if (efSystem_) efSystem_->Uninit();
}

void SelectSkillScene::Update()
{
	if (!sceneChange_)
	{
		//コントローラースティック傾き
		float controllerLX = (float)X_CONTROLLER::GetXcontrollerLStickX(1);

		//選択中のスキル変更
		if (GetKeyboardTrigger(DIK_A) || GetKeyboardTrigger(DIK_LEFT) ||
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_LEFT))
		{
			if (selectingNumber_ != 0)	selectingNumber_--;
			//現在選んでいるスキルがすでに選択されていた場合
			if (selectingNumber_ == selectedSkill1_) {
				selectingNumber_--;
				if (selectingNumber_ == -1)selectingNumber_ += 2;
			}
		}
		if (GetKeyboardTrigger(DIK_D) || GetKeyboardTrigger(DIK_RIGHT) ||
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_RIGHT))
		{
			if (selectingNumber_ != (int)SelectSkillScene::SKILL_MAX - 1) selectingNumber_++;
			//現在選んでいるスキルがすでに選択されていた場合
			if (selectingNumber_ == selectedSkill1_) {
				selectingNumber_++;
				if (selectingNumber_ == SKILL_MAX)selectingNumber_ -= 2;
			}
		}
		//スティックスキル選択
		if (controllerLX < -0.1f && isActiveControllerStick_) {
			isActiveControllerStick_ = false;
			if (selectingNumber_ != 0)	selectingNumber_--;
		}
		if (controllerLX > 0.1f && isActiveControllerStick_) {
			isActiveControllerStick_ = false;
			if (selectingNumber_ != SelectSkillScene::SKILL_MAX - 1) selectingNumber_++;
		}
		//スキル決定
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) ||
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_B))
		{
			if (selectedSkill1_ == -1) selectedSkill1_ = selectingNumber_;
			else selectedSkill2_ = selectingNumber_;

			//現在選んでいるスキルがすでに選択されていた場合
			if (selectingNumber_ == selectedSkill1_ || selectingNumber_ == selectedSkill2_) {
				selectingNumber_++;
				if (selectingNumber_ == SKILL_MAX)selectingNumber_ -= 2;
			}
		}

		if (uiSystem_)	uiSystem_->Update();		//UI更新
		if (efSystem_) efSystem_->Update();			//エフェクト更新

		//スキルキャンセル
		if (GetKeyboardTrigger(DIK_B) || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_A))
		{
			if (selectedSkill2_ != -1) {
				uiSystem_->ResetIconColor(selectedSkill2_);
				selectedSkill2_ = -1;
			}
			else if (selectedSkill1_ != -1) {
				uiSystem_->ResetIconColor(selectedSkill1_);
				selectedSkill1_ = -1;
			}
		}

		if (selectingOldNumber_ != selectingNumber_) selectingOldNumber_ = selectingNumber_;
		if (!isActiveControllerStick_)controllerCounter_++;
		if (controllerCounter_ >= CONTROLLER_COUNTER_VALUE) {
			controllerCounter_ = 0;
			isActiveControllerStick_ = true;
		}

		if (selectedSkill1_ != -1 && selectedSkill2_ != -1)
		{
			GameManager::SetPlayerSkill1(selectedSkill1_);
			GameManager::SetPlayerSkill2(selectedSkill2_);
			sceneChange_ = true;
			fade_->FadeIn();
		}
	}
	else
	{
		if (!fade_->IsFading())
		{
			Object::ReleaseAll();
			GameManager::SetScene(new GameScene());
			return;
		}
	}
	Object::UpdateAll();
}

void SelectSkillScene::BeginDraw()
{
	Object::BeginDrawAll();
}

void SelectSkillScene::Draw()
{
	if (uiSystem_)	uiSystem_->Draw();
	if (efSystem_) efSystem_->Draw();
	Object::DrawAll();
}

void SelectSkillScene::EndDraw()
{
	Object::EndDrawAll();
}
