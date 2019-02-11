#include "GameCountdown.h"
#include "../../Texture/Texture.h"
#include "../../main.h"
#include "../../Sound/Sound.h"

constexpr float WORD_INCSPEED = 10.0f;			//文字の拡大スピード
constexpr float WORD_MAXSIZE = 600.0f;			//文字の最大の大きさ
constexpr int BACK_ALPHA = 150;						//背景の透明度

GameCountdown::GameCountdown()
{
	back_ = new Texture(TextureManager::Tex_Fade);
	count1_ = new Texture(TextureManager::Tex_Count1);
	count2_ = new Texture(TextureManager::Tex_Count2);
	count3_ = new Texture(TextureManager::Tex_Count3);
	countSE_ = new Sound(SoundManager::COUNT_SE);
	startSE_ = new Sound(SoundManager::GAMESTART_SE);
}

GameCountdown::~GameCountdown()
{
	if (back_)
	{
		back_->Uninit();
		SAFE_DELETE(back_);
	}
	if (count1_)
	{
		count1_->Uninit();
		SAFE_DELETE(count1_);
	}
	if (count2_)
	{
		count2_->Uninit();
		SAFE_DELETE(count2_);
	}
	if (count3_)
	{
		count3_->Uninit();
		SAFE_DELETE(count3_);
	}
	if (countSE_)
	{
		countSE_->StopSound();
		SAFE_DELETE(countSE_);
	}
	if (startSE_)
	{
		startSE_->StopSound();
		SAFE_DELETE(startSE_);
	}
}

void GameCountdown::Init()
{
	wordSize_ = 0.0f;			//文字サイズ
	index_ = 3;					//最初に表示する数字
	isCountDown_ = true;

	back_->Init();
	count1_->Init();
	count2_->Init();
	count3_->Init();

	count1_->SetVisible(false);
	count2_->SetVisible(false);
	count3_->SetVisible(false);
	back_->SetVisible(true);

	back_->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	count1_->SetDrawSize(wordSize_,wordSize_);
	count2_->SetDrawSize(wordSize_, wordSize_);
	count3_->SetDrawSize(wordSize_, wordSize_);

	back_->SetColor(D3DCOLOR_RGBA(255,255,255,BACK_ALPHA));
}

void GameCountdown::Uninit()
{
	count1_->Uninit();
	count2_->Uninit();
	count3_->Uninit();
	back_->Uninit();
}

void GameCountdown::Update()
{
	if (index_ > 0)
	{
		count1_->SetVisible(false);
		count2_->SetVisible(false);

		wordSize_ += WORD_INCSPEED;
		if (wordSize_ >= WORD_MAXSIZE)
		{
			wordSize_ = 0.0f;
			index_--;
			countSE_->PlaySoundA();
		}
		float draw_positionX = ((float)ScreenWidth - wordSize_) / 2.0f;
		float draw_positionY = ((float)ScreenHeight - wordSize_) / 2.0f;

		switch (index_)
		{
		case 3:
			count3_->SetVisible(true);
			count3_->SetDrawSize(wordSize_,wordSize_);
			count3_->SetPosition(draw_positionX,draw_positionY,0.0f);
			break;
		case 2:
			count2_->SetVisible(true);
			count2_->SetDrawSize(wordSize_, wordSize_);
			count2_->SetPosition(draw_positionX, draw_positionY, 0.0f);
			count3_->SetVisible(false);
			break;
		case 1:
			count1_->SetVisible(true);
			count1_->SetDrawSize(wordSize_, wordSize_);
			count1_->SetPosition(draw_positionX, draw_positionY, 0.0f);
			count2_->SetVisible(false);
			break;
		default:
			count1_->SetVisible(false);
			back_->SetVisible(false);
			startSE_->PlaySoundA();
			break;
		}
	}
	else if (isCountDown_)
	{
		isCountDown_ = false;
	}
}

void GameCountdown::Draw()
{
	if (index_ > 0)
	{
		back_->Draw();
		count1_->Draw();
		count2_->Draw();
		count3_->Draw();
	}
}
