#include "TitleSelect.h"
#include "../../Texture/Texture.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"
#include "../../main.h"
#include "../../Sound/Sound.h"
#include "../../Game/MouseState/MouseState.h"
#include "../TitleScene.h"
#include "../../Imgui/ImguiManager.h"

const float TextureWidth = 300.0f;						//テクスチャ幅
const float TextureHeigt = 100.0f;						//テクスチャ高さ
const float OffsetHeight = 200.0f;						//テクスチャ初期高さオフセット
const int NotSelectAlpha = 150;						//選択中でないときの透明度
const float ScaleRateMax = 1.5f;						//テクスチャスケール最大比率
const float ScaleRateAdd = 0.015f;					//テクスチャ毎フレーム加算比率
const float ControllerCounterValue = 20;			//コントローラーカウンタ

TitleSelect::TitleSelect(TitleScene* scene)
{
	titleScene_ = scene;

	selectingSE_ = new Sound(SoundManager::SELECTING_SE);

	selectMode_ = SELECT_MODE::EDIT;
	edit_ = Object::Create<Texture>(TextureManager::Tex_EditWord);
	game_ = Object::Create<Texture>(TextureManager::Tex_GameWord);
	exit_ = Object::Create<Texture>(TextureManager::Tex_ExitWord);
}

TitleSelect::~TitleSelect()
{
	if (selectingSE_)
	{
		selectingSE_->StopSound();
		delete selectingSE_;
		selectingSE_ = nullptr;
	}
}

void TitleSelect::Init()
{
	edit_->SetDrawSize(TextureWidth,TextureHeigt);
	game_->SetDrawSize(TextureWidth,TextureHeigt);
	exit_->SetDrawSize(TextureWidth,TextureHeigt);
}

void TitleSelect::Uninit()
{
}

void TitleSelect::Update()
{
	//コントローラースティック傾き
	float controllerLY = (float)X_CONTROLLER::GetXcontrollerLStickY(1);

	if (!isActiveControllerStick_) {
		controllerCounter_++;
		if (controllerCounter_ >= ControllerCounterValue) {
			controllerCounter_ = 0;
			isActiveControllerStick_ = true;
		}
	}

	if (selectMode_ > 0)
	{
		if (GetKeyboardTrigger(DIK_W) || GetKeyboardTrigger(DIK_UP) || 
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_UP))
		{
			selectingSE_->PlaySoundA();
			selectMode_--;
			scaleRate_ = 1.0f;
		}
		if (controllerLY > 0.1f && isActiveControllerStick_)
		{
			selectingSE_->PlaySoundA();
			selectMode_--;
			scaleRate_ = 1.0f;
			isActiveControllerStick_ = false;
		}
	}
	if (selectMode_ < MODE_MAX - 1)
	{
		if (GetKeyboardTrigger(DIK_S) || GetKeyboardTrigger(DIK_DOWN) || 
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_DPAD_DOWN))
		{
			selectingSE_->PlaySoundA();
			selectMode_++;
			scaleRate_ = 1.0f;
		}
		if (controllerLY < -0.1f && isActiveControllerStick_)
		{
			selectingSE_->PlaySoundA();
			selectMode_++;
			scaleRate_ = 1.0f;
			isActiveControllerStick_ = false;
		}
	}
	
	scaleRate_ += ScaleRateAdd;

	switch (selectMode_)
	{
	case EDIT:
		edit_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		game_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));
		exit_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));

		edit_->SetDrawSize(TextureWidth * scaleRate_, TextureHeigt * scaleRate_);
		game_->SetDrawSize(TextureWidth, TextureHeigt);
		exit_->SetDrawSize(TextureWidth, TextureHeigt);
		break;
	case GAME:
		edit_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));
		game_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));
		exit_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));

		edit_->SetDrawSize(TextureWidth, TextureHeigt);
		game_->SetDrawSize(TextureWidth * scaleRate_, TextureHeigt * scaleRate_);
		exit_->SetDrawSize(TextureWidth, TextureHeigt);
		break;
	case EXIT:
		edit_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));
		game_->SetColor(D3DCOLOR_RGBA(255, 255, 255, NotSelectAlpha));
		exit_->SetColor(D3DCOLOR_RGBA(255, 100, 100, 255));

		edit_->SetDrawSize(TextureWidth, TextureHeigt);
		game_->SetDrawSize(TextureWidth, TextureHeigt);
		exit_->SetDrawSize(TextureWidth * scaleRate_, TextureHeigt * scaleRate_);
		break;
	default:
		break;
	}

	edit_->SetPosition((float)(ScreenWidth - edit_->GetDrawSize().x) / 2.0f, (float)(ScreenHeight - edit_->GetDrawSize().y) / 2.0f + TextureHeigt * SELECT_MODE::EDIT + OffsetHeight, 0.0f);
	game_->SetPosition((float)(ScreenWidth - game_->GetDrawSize().x) / 2.0f, (float)(ScreenHeight - game_->GetDrawSize().y) / 2.0f + TextureHeigt * SELECT_MODE::GAME + OffsetHeight, 0.0f);
	exit_->SetPosition((float)(ScreenWidth - exit_->GetDrawSize().x) / 2.0f, (float)(ScreenHeight - exit_->GetDrawSize().y) / 2.0f + TextureHeigt * SELECT_MODE::EXIT + OffsetHeight, 0.0f);

	//マウスでの当たり判定
	DIMOUSESTATE MouseData = ReturnMouseMove();
	
	if (MouseState::IsHitMouseCursorBox(edit_->GetPosition().x, edit_->GetPosition().y, edit_->GetDrawSize().x, edit_->GetDrawSize().y))
	{
		if (selectMode_ != EDIT)
		{
			selectMode_ = EDIT;
			scaleRate_ = 1.0f;
			selectingSE_->PlaySoundA();
		}
		//クリックした際の処理
		if (MouseData.rgbButtons[0] == 0x80 && !isClicked_)
		{
			isClicked_ = true;
			titleScene_->SceneChange();
		}
	}
	else if (MouseState::IsHitMouseCursorBox(game_->GetPosition().x, game_->GetPosition().y, game_->GetDrawSize().x, game_->GetDrawSize().y))
	{
		if (selectMode_ != GAME)
		{
			selectMode_ = GAME;
			scaleRate_ = 1.0f;
			selectingSE_->PlaySoundA();
		}
		//クリックした際の処理
		if (MouseData.rgbButtons[0] == 0x80 && !isClicked_)
		{
			isClicked_ = true;
			titleScene_->SceneChange();
		}
	}
	else if (MouseState::IsHitMouseCursorBox(exit_->GetPosition().x, exit_->GetPosition().y, exit_->GetDrawSize().x, exit_->GetDrawSize().y))
	{
		if (selectMode_ != EXIT)
		{
			selectMode_ = EXIT;
			scaleRate_ = 1.0f;
			selectingSE_->PlaySoundA();
		}
		//クリックした際の処理
		if (MouseData.rgbButtons[0] == 0x80 && !isClicked_)
		{
			isClicked_ = true;
			titleScene_->SceneChange();
		}
	}

	if (scaleRate_ >= ScaleRateMax)
	{
		scaleRate_ = 1.0f;
	}


}

void TitleSelect::Draw()
{
}
