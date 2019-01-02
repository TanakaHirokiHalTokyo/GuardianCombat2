#include "ResultScene.h"
#include "../Game/GameManager/GameManager.h"
#include "TitleScene.h"
#include "../DInput/DirectInput.h"
#include "Fade.h"
#include "../Texture/Texture.h"
#include "../main.h"
#include "../Sound/Sound.h"

constexpr int SCENE_CHANGE_COUNT = GameFPS * 6;

ResultScene::ResultScene()
{
	GameManager::SetSceneTag("ResultScene");
	GameManager::GetFade()->FadeOut();

	bgm_ = new Sound(SoundManager::RESULT_BGM);
	bgm_->PlaySoundA();
}

ResultScene::~ResultScene()
{
	if (bgm_)
	{
		bgm_->StopSound();
		delete bgm_;
		bgm_ = nullptr;
	}
}

void ResultScene::Init()
{
	Texture* thanks = Object::Create<Texture>(TextureManager::Tex_Thanks);
	thanks->SetDrawSize((float)ScreenWidth, (float)ScreenHeight);
	Object::InitAll();
}

void ResultScene::Uninit()
{
	Object::ReleaseAll();
}

void ResultScene::Update()
{
	//Fadeポインタ取得
	Fade* fade = GameManager::GetFade();

	//シーンチェンジ
	if (!sceneChange_)
	{
		count_++;
		if (count_ >= SCENE_CHANGE_COUNT)
		{
			fade->FadeIn();
			sceneChange_ = true;
		}

		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN))
		{
			fade->FadeIn();
			sceneChange_ = true;
		}
	}
	else
	{
		if (ChangeSceneUpdate())
		{
			GameManager::SetScene(new TitleScene());
		}
	}
	Object::UpdateAll();
}

void ResultScene::BeginDraw()
{
	Object::BeginDrawAll();
}

void ResultScene::Draw()
{
	Object::DrawAll();
}

void ResultScene::EndDraw()
{
	Object::EndDrawAll();
}
