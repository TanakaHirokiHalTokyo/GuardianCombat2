#include "TitleScene.h"
#include "../DInput/DirectInput.h"
#include "../Game/GameManager/GameManager.h"
#include "GameScene.h"
#include "Fade.h"
#include "../Game/Object.h"

TitleScene::TitleScene()
{
	//SceneTag‚ðÝ’è
	GameManager::SetSceneTag("TitleScene");
	//FadeOut
	GameManager::GetFade()->FadeOut();
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	Object::InitAll();
}

void TitleScene::Uninit()
{
}

void TitleScene::Update()
{
	Fade* fade = GameManager::GetFade();

	if (!sceneChange_)
	{
		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
		{
			fade->FadeIn();
			sceneChange_ = true;
		}
	}
	else
	{
		if (ChangeSceneUpdate())
		{
			GameManager::SetScene(new GameScene());
		}
	}
	
}

void TitleScene::BeginDraw()
{
}

void TitleScene::Draw()
{
}

void TitleScene::EndDraw()
{
}
