#include "ResultScene.h"
#include "../Game/GameManager/GameManager.h"
#include "TitleScene.h"
#include "../DInput/DirectInput.h"
#include "Fade.h"

ResultScene::ResultScene()
{
	GameManager::SetSceneTag("ResultScene");
	GameManager::GetFade()->FadeOut();
}

ResultScene::~ResultScene()
{
}

void ResultScene::Init()
{
}

void ResultScene::Uninit()
{
}

void ResultScene::Update()
{
	//Fadeポインタ取得
	Fade* fade = GameManager::GetFade();

	//シーンチェンジ
	if (!sceneChange_)
	{
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
}

void ResultScene::BeginDraw()
{
}

void ResultScene::Draw()
{
}

void ResultScene::EndDraw()
{
}
