#include "PauseSetting.h"
#include "../../Texture/Texture.h"
#include "../../DInput/DirectInput.h"
#include "../../Game/Player/Player.h"
#include "../../Game/Player/DeviceConfig/DeviceConfig.h"
#include "../../Game/GameManager/GameManager.h"
#include "../../Imgui/ImguiManager.h"
#include "../../Game/MouseState/MouseState.h"
#include "../../main.h"
#include "PauseSelect.h"
#include <fstream>

const std::string FILENAME = "PauseDeviceSettings";
const std::string PauseSetting::SENCITIVITY_FILENAME = "UserSensitivity";

PauseSetting::PauseSetting(PauseSelect* select)
{
	select_ = select;	
	this->LoadSettings(FILENAME);
	this->LoadSensitivity(SENCITIVITY_FILENAME);
	bar_ = new Texture(TextureManager::Tex_SliderBar);
	circle_ = new Texture(TextureManager::Tex_SliderButton);
	b_ = new Texture(TextureManager::Tex_B);
	back_ = new Texture(TextureManager::Tex_Back);
}

PauseSetting::~PauseSetting()
{
	SAFE_DELETE(bar_);
	SAFE_DELETE(circle_);
	SAFE_DELETE(b_);
	SAFE_DELETE(back_);
}

void PauseSetting::Init()
{
	bar_->Init();
	circle_->Init();
	b_->Init();
	back_->Init();

	//座標をウィンドウサイズに合わせる
	float px = barPosition_.x * ((float)ScreenWidth / 1600.0f);
	float py = barPosition_.y * ((float)ScreenHeight / 900.0f);
	//サイズをウィンドウサイズに合わせる
	float sx = barSize_.x * ((float)ScreenWidth / 1600.0f);
	float sy = barSize_.y * ((float)ScreenHeight / 900.0f);

	bar_->SetPosition(px, py, 0.0f);
	bar_->SetDrawSize(sx, sy);
	bar_->SetColor(D3DCOLOR_RGBA((int)(barColor_[0] * 255.0f), (int)(barColor_[1] * 255.0f), (int)(barColor_[2] * 255.0f), 255));

	//座標をウィンドウサイズに合わせる
	px = circlePosition_.x * ((float)ScreenWidth / 1600.0f);
	py = circlePosition_.y * ((float)ScreenHeight / 900.0f);
	//サイズをウィンドウサイズに合わせる
	sx = circleSize_.x * ((float)ScreenWidth / 1600.0f);
	sy = circleSize_.y * ((float)ScreenHeight / 900.0f);

	circle_->SetPosition(px, py, 0.0f);
	circle_->SetDrawSize(sx, sy);
	circle_->SetColor(D3DCOLOR_RGBA((int)(circleColor_[0] * 255.0f), (int)(circleColor_[1] * 255.0f), (int)(circleColor_[2] * 255.0f), 255));

	b_->SetPosition(bPosition_.x, bPosition_.y, 0.0f);
	b_->SetDrawSize(bSize_.x, bSize_.y);
	b_->SetColor(D3DCOLOR_RGBA((int)(bColor_[0] * 255.0f), (int)(bColor_[1] * 255.0f), (int)(bColor_[2] * 255.0f), 255));

	back_->SetPosition(backPosition_.x, backPosition_.y, 0.0f);
	back_->SetDrawSize(backSize_.x, backSize_.y);
	back_->SetColor(D3DCOLOR_RGBA((int)(backColor_[0] * 255.0f), (int)(backColor_[1] * 255.0f), (int)(backColor_[2] * 255.0f), 255));
}

void PauseSetting::Uninit()
{
}

void PauseSetting::Update()
{
	//config_がnullptrの場合Config取得
	if(!config_)	config_ = GameManager::GetPlayer()->GetDeviceConfig();
	if (!config_) return;		//取得に失敗した場合Update終了

								//座標をウィンドウサイズに合わせる
	float px = barPosition_.x * ((float)ScreenWidth / 1600.0f);
	float py = barPosition_.y * ((float)ScreenHeight / 900.0f);
	//サイズをウィンドウサイズに合わせる
	float sx = barSize_.x * ((float)ScreenWidth / 1600.0f);
	float sy = barSize_.y * ((float)ScreenHeight / 900.0f);

	bar_->SetPosition(px, py, 0.0f);
	bar_->SetDrawSize(sx, sy);
	bar_->SetColor(D3DCOLOR_RGBA((int)(barColor_[0] * 255.0f), (int)(barColor_[1] * 255.0f), (int)(barColor_[2] * 255.0f), 255));

	POINT ms = MouseState::GetMouseCursor();
	DIMOUSESTATE MouseData = ReturnMouseMove();

	//マウスクリックでボタン移動
	if (MouseData.rgbButtons[0] == 0x80)
	{
		if (MouseState::IsHitMouseCursorBox(circlePosition_.x, circlePosition_.y,circleSize_.x,circleSize_.y) || isClicked_)
		{
			isClicked_ = true;

			circlePosition_.x = (float)ms.x;
			if (circlePosition_.x > bar_->GetPosition().x + bar_->GetDrawSize().x)
			{
				circlePosition_.x = bar_->GetPosition().x + bar_->GetDrawSize().x;
			}
			else if (circlePosition_.x < bar_->GetPosition().x)
			{
				circlePosition_.x = bar_->GetPosition().x;
			}

			//座標をウィンドウサイズに合わせる
			px = circlePosition_.x * ((float)ScreenWidth / 1600.0f);
			py = circlePosition_.y * ((float)ScreenHeight / 900.0f);
			//サイズをウィンドウサイズに合わせる
			sx = circleSize_.x * ((float)ScreenWidth / 1600.0f);
			sy = circleSize_.y * ((float)ScreenHeight / 900.0f);

			circle_->SetPosition(px - circleSize_.x / 2.0f, py, 0.0f);
			circle_->SetDrawSize(sx, sy);
			circle_->SetColor(D3DCOLOR_RGBA((int)(circleColor_[0] * 255.0f), (int)(circleColor_[1] * 255.0f), (int)(circleColor_[2] * 255.0f), 255));

			//マウス感度設定
			if(device_ == MOUSE)	mouse_rate_ = (circlePosition_.x - barPosition_.x) / barSize_.x;
			else if(device_ == CONTROLLER) controller_rate_ = (circlePosition_.x - barPosition_.x) / barSize_.x;
			
			switch (device_)
			{
			case PauseSetting::MOUSE:
				config_->ChangeMouseSensitivity(config_->MouseMaxSensitivity * mouse_rate_);
				break;
			case PauseSetting::CONTROLLER:
				config_->ChangeControllerSensitivity(config_->ControllerMaxSensitivity * controller_rate_);
				break;
			default:
				break;
			}
		}
		
	}
	else if (isClicked_)
	{
		isClicked_ = false;
	}

	//デバイス選択画面に戻る
	if (MouseData.rgbButtons[0] == 0x80)
	{
		if (MouseState::IsHitMouseCursorBox(b_->GetPosition().x, b_->GetPosition().y, b_->GetDrawSize().x, b_->GetDrawSize().y))
		{
			select_->Selecting();
			SaveSensitivity(SENCITIVITY_FILENAME);
		}
	}

	if (GameManager::GetEnableEdit())
	{
		ImGui::Begin(u8"PauseSetting編集");
		ImGui::Text(u8"マウス感度 : %f",this->mouse_rate_);
		if (ImGui::Button(u8"保存")) SaveSettings(FILENAME);
		ImGui::DragFloat(u8"バーのX座標", &barPosition_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"バーのY座標", &barPosition_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::DragFloat(u8"バーの幅",&barSize_.x,1.0f,0.0f,(float)ScreenWidth);
		ImGui::DragFloat(u8"バーの高さ", &barSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::ColorEdit3(u8"バーの色",barColor_);

		ImGui::DragFloat(u8"ボタンのX座標", &circlePosition_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"ボタンのY座標", &circlePosition_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::DragFloat(u8"ボタンの幅", &circleSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"ボタンの高さ", &circleSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::ColorEdit3(u8"ボタンの色",circleColor_);

		ImGui::DragFloat(u8"BのX座標", &bPosition_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"BのY座標", &bPosition_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::DragFloat(u8"Bの幅", &bSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"Bの高さ", &bSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::ColorEdit3(u8"Bの色", bColor_);

		ImGui::DragFloat(u8"BackのX座標", &backPosition_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"BackのY座標", &backPosition_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::DragFloat(u8"Backの幅", &backSize_.x, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat(u8"Backの高さ", &backSize_.y, 1.0f, 0.0f, (float)ScreenHeight);
		ImGui::ColorEdit3(u8"Backの色", backColor_);
		ImGui::End();
		
		b_->SetPosition(bPosition_.x,bPosition_.y,0.0f);
		b_->SetDrawSize(bSize_.x,bSize_.y);
		b_->SetColor(D3DCOLOR_RGBA((int)(bColor_[0] * 255.0f), (int)(bColor_[1] * 255.0f), (int)(bColor_[2] * 255.0f), 255));

		back_->SetPosition(backPosition_.x, backPosition_.y, 0.0f);
		back_->SetDrawSize(backSize_.x, backSize_.y);
		back_->SetColor(D3DCOLOR_RGBA((int)(backColor_[0] * 255.0f), (int)(backColor_[1] * 255.0f), (int)(backColor_[2] * 255.0f), 255));
	}
}

void PauseSetting::Draw()
{
	if(bar_)	bar_->Draw();
	if (circle_) circle_->Draw();
	if (b_)b_->Draw();
	if (back_)back_->Draw();
}

void PauseSetting::SaveSettings(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::out);
	file.write((const char*)&this->barPosition_,sizeof(this->barPosition_));
	file.write((const char*)&this->barSize_, sizeof(this->barSize_));
	file.write((const char*)&this->barColor_, sizeof(this->barColor_));
	file.write((const char*)&this->circlePosition_, sizeof(this->circlePosition_));
	file.write((const char*)&this->circleSize_, sizeof(this->circleSize_));
	file.write((const char*)&this->circleColor_, sizeof(this->circleColor_));
	file.write((const char*)&this->bPosition_, sizeof(this->bPosition_));
	file.write((const char*)&this->bSize_, sizeof(this->bSize_));
	file.write((const char*)&this->bColor_, sizeof(this->bColor_));
	file.write((const char*)&this->backPosition_, sizeof(this->backPosition_));
	file.write((const char*)&this->backSize_, sizeof(this->backSize_));
	file.write((const char*)&this->backColor_, sizeof(this->backColor_));
	file.close();
}

void PauseSetting::LoadSettings(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "デバイス設定画面データを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->barPosition_, sizeof(this->barPosition_));
		file.read((char*)&this->barSize_, sizeof(this->barSize_));
		file.read((char*)&this->barColor_, sizeof(this->barColor_));
		file.read((char*)&this->circlePosition_, sizeof(this->circlePosition_));
		file.read((char*)&this->circleSize_, sizeof(this->circleSize_));
		file.read((char*)&this->circleColor_, sizeof(this->circleColor_));
		file.read((char*)&this->bPosition_, sizeof(this->bPosition_));
		file.read((char*)&this->bSize_, sizeof(this->bSize_));
		file.read((char*)&this->bColor_, sizeof(this->bColor_));
		file.read((char*)&this->backPosition_, sizeof(this->backPosition_));
		file.read((char*)&this->backSize_, sizeof(this->backSize_));
		file.read((char*)&this->backColor_, sizeof(this->backColor_));
	}
	file.close();
}
void PauseSetting::SaveSensitivity(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".gc2",std::ios::binary | std::ios::out);
	file.write((const char*)&this->mouse_rate_,sizeof(mouse_rate_));
	file.write((const char*)&this->controller_rate_,sizeof(this->controller_rate_));
	file.close();
}
void PauseSetting::SetCirclePosition()
{
	if (device_ == MOUSE)
	{
		circlePosition_.x = barPosition_.x + barSize_.x * mouse_rate_;
	}
	else if (device_ == CONTROLLER)
	{
		circlePosition_.x = barPosition_.x + barSize_.x * controller_rate_;
	}
	circle_->SetPosition(circlePosition_.x,circlePosition_.y,0.0f);
}
void PauseSetting::LoadSensitivity(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "デバイス設定画面データを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->mouse_rate_, sizeof(mouse_rate_));
		file.read((char*)&this->controller_rate_,sizeof(controller_rate_));
	}
	file.close();
}
