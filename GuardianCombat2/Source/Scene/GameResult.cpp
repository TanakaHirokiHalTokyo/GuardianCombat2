#include "GameResult.h"
#include "../Texture/Texture.h"
#include "Fade.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"
#include "ResultScene.h"
#include "../Imgui/ImguiManager.h"
#include "../main.h"
#include "../Sound/Sound.h"
#include <fstream>

const float WORD_SIZE_WIDTH = 600.0f * ((float)ScreenWidth / 1600.0f);				//デフォルトワードサイズ幅
const float WORD_SIZE_HEGIHT = 200.0f * ((float)ScreenHeight / 900.0f);			//デフォルトワードサイズ高さ
const float WORD_MAXRATE = 2.0f;																			//ワード最大値割合
const float WORD_ADD_SPEED = 0.01f;																	//ワード拡大スピード
const float BACK_SIZE_WIDTH = 1600.0f * ((float)ScreenWidth / 1600.0f);			//背景サイズ幅
const float BACK_SIZE_HEIGHT = 900.0f * ((float)ScreenHeight / 900.0f);				//背景サイズ高さ
const float KEY_SIZE_WIDTH = 600.0f * ((float)ScreenWidth / 1600.0f);					//PressKeyサイズ幅
const float KEY_SIZE_HEIGHT = 200.0f * ((float)ScreenHeight / 900.0f);				//PressKeyサイズ高さ

const char* RESULT_WORD[2] =
{
	"Win",
	"Lose"
};

GameResult::GameResult()
{
	this->LoadSettings("GameResultSettings");
	GameManager::GetFade()->FadeOut();
	bgm_ = new Sound(SoundManager::GAMERESULT_BGM);
	selectSE_ = new Sound(SoundManager::SELECTOK_SE);
	resultBack_ = Object::Create<Texture>(TextureManager::Tex_GameResultBack);
	pressKey_ = Object::Create<Texture>(TextureManager::Tex_PressSpace);
	if (GameManager::IsGameClear()) { resultWord_ = Object::Create<Texture>(TextureManager::Tex_Win);	selectResultNumber_ = 0; }
	else { resultWord_ = Object::Create<Texture>(TextureManager::Tex_Lose);		selectResultNumber_ = 1; }
}

GameResult::~GameResult()
{
	if (bgm_)
	{
		bgm_->StopSound();
		SAFE_DELETE(bgm_);
	}
	if (selectSE_)
	{
		selectSE_->StopSound();
		SAFE_DELETE(selectSE_);
	}
}

void GameResult::Init()
{
	bgm_->PlaySoundA();
	scaleAddRate_ = WORD_ADD_SPEED;
	resultWord_->SetDrawSize(WORD_SIZE_WIDTH,WORD_SIZE_HEGIHT);
	resultBack_->SetDrawSize(BACK_SIZE_WIDTH,BACK_SIZE_HEIGHT);
	pressKey_->SetDrawSize(KEY_SIZE_WIDTH,KEY_SIZE_HEIGHT);
	pressKey_->SetPosition(((float)ScreenWidth - KEY_SIZE_WIDTH) / 2.0f, (float)ScreenHeight - KEY_SIZE_HEIGHT, 0.0f);
	Object::InitAll();
}

void GameResult::Uninit()
{

}

void GameResult::Update()
{
	//結果表示画面編集
	if (GameManager::GetEnableEdit())
	{
		ImGui::Begin(u8"ゲームリザルト編集");
		if (ImGui::Button(u8"保存"))
		{
			this->SaveSettings("GameResultSettings");
		}
		if (ImGui::ListBox(u8"結果選択", &selectResultNumber_, RESULT_WORD, IM_ARRAYSIZE(RESULT_WORD), 2))
		{
			switch (selectResultNumber_)
			{
			case 0:			//Win
				resultWord_->SetTexture(TextureManager::Tex_Win);
				break;
			case 1:			//Lose
				resultWord_->SetTexture(TextureManager::Tex_Lose);
				break;
			default:
				break;
			}
		}
		//文字色設定
		if (selectResultNumber_ == 0)	ImGui::ColorEdit3(u8"勝利文字色",wordWinColor_);
		else ImGui::ColorEdit3(u8"敗北文字色",wordLoseColor_)	;
		//背景色設定
		if (selectResultNumber_ == 0)		ImGui::ColorEdit3(u8"勝利背景色",backWinColor_);
		else	ImGui::ColorEdit3(u8"勝利背景色", backLoseColor_);
		//PressKey設定
		ImGui::ColorEdit3(u8"PressKey文字色",keyColor_);
		ImGui::SliderFloat(u8"点滅スピード",&this->alphaAddRate_,0.1f,10.0f);
		ImGui::End();
	}

	//文字色更新
	if (selectResultNumber_ == 0)		resultWord_->SetColor(D3DCOLOR_RGBA((int)(wordWinColor_[0] * 255.0f), (int)(wordWinColor_[1] * 255.0f), (int)(wordWinColor_[2] * 255.0f), 255));
	else	resultWord_->SetColor(D3DCOLOR_RGBA((int)(wordLoseColor_[0] * 255.0f), (int)(wordLoseColor_[1] * 255.0f), (int)(wordLoseColor_[2] * 255.0f), 255));
	//背景色更新
	if (selectResultNumber_ == 0)	resultBack_->SetColor(D3DCOLOR_RGBA((int)(backWinColor_[0] * 255.0f), (int)(backWinColor_[1] * 255.0f), (int)(backWinColor_[2] * 255.0f), 255));
	else	resultBack_->SetColor(D3DCOLOR_RGBA((int)(backLoseColor_[0] * 255.0f), (int)(backLoseColor_[1] * 255.0f), (int)(backLoseColor_[2] * 255.0f), 255));

	//拡大率更新
	if (scaleAddRate_ > 0.0f)
	{
		scaleRate_ += scaleAddRate_;
		if (scaleRate_ >= WORD_MAXRATE) scaleAddRate_ *= -1.0f;
	}
	else
	{
		scaleRate_ += scaleAddRate_;
		if (scaleRate_ <= 1.0f) scaleAddRate_ *= -1.0f;
	}
	//α値更新
	if (alphaAddRate_ > 0.0f)
	{
		if (keyAlpha_ >= 255.0f) {
			alphaAddRate_ *= -1.0f;
			keyAlpha_ = 255.0f;
		}
		keyAlpha_ += alphaAddRate_;
	}
	else
	{
		if (keyAlpha_ <= 0.0f) {
			alphaAddRate_ *= -1.0f;
			keyAlpha_ = 0.0f;
		}
		keyAlpha_ += alphaAddRate_;
	}

	resultWord_->SetDrawSize(WORD_SIZE_WIDTH * scaleRate_, WORD_SIZE_HEGIHT * scaleRate_);
	resultWord_->SetPosition(((float)ScreenWidth - resultWord_->GetDrawSize().x) / 2.0f,((float)ScreenHeight - resultWord_->GetDrawSize().y) / 2.0f,0.0f);

	resultBack_->SetDrawSize(BACK_SIZE_WIDTH * scaleRate_, BACK_SIZE_HEIGHT * scaleRate_);
	resultBack_->SetPosition(((float)ScreenWidth - resultBack_->GetDrawSize().x) / 2.0f, ((float)ScreenHeight - resultBack_->GetDrawSize().y) / 2.0f, 0.0f);

	//Press文字色更新
	pressKey_->SetColor(D3DCOLOR_RGBA((int)(keyColor_[0] * 255.0f), (int)(keyColor_[1] * 255.0f), (int)(keyColor_[2] * 255.0f), (int)keyAlpha_));

	//Fadeポインタ取得
	Fade* fade = GameManager::GetFade();
	//シーンチェンジ
	if (!sceneChange_)
	{
		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN))
		{
			fade->FadeIn();
			selectSE_->PlaySoundA();
			sceneChange_ = true;
		}
	}
	else
	{
		if (ChangeSceneUpdate())
		{
			//オブジェクト解放
			Object::ReleaseAll();
			GameManager::SetScene(new ResultScene());
			return;
		}
	}

	Object::UpdateAll();
}

void GameResult::BeginDraw()
{
	Object::BeginDrawAll();
}

void GameResult::Draw()
{
	Object::DrawAll();
}

void GameResult::EndDraw()
{
	Object::EndDrawAll();
}

void GameResult::SaveSettings(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::out);
	file.write((const char*)&this->wordWinColor_, sizeof(this->wordWinColor_));
	file.write((const char*)&this->wordLoseColor_, sizeof(this->wordLoseColor_));
	file.write((const char*)&this->backWinColor_,sizeof(this->backWinColor_));
	file.write((const char*)&this->backLoseColor_, sizeof(this->backLoseColor_));
	file.write((const char*)&this->keyColor_, sizeof(this->keyColor_));
	file.close();
}

void GameResult::LoadSettings(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "ゲームリザルトデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->wordWinColor_, sizeof(this->wordWinColor_));
		file.read((char*)&this->wordLoseColor_, sizeof(this->wordLoseColor_));
		file.read((char*)&this->backWinColor_, sizeof(this->backWinColor_));
		file.read((char*)&this->backLoseColor_, sizeof(this->backLoseColor_));
		file.read((char*)&this->keyColor_, sizeof(this->keyColor_));
	}
	file.close();
}
