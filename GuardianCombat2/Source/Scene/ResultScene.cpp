#include "ResultScene.h"
#include "../Game/GameManager/GameManager.h"
#include "TitleScene.h"
#include "../DInput/DirectInput.h"

ResultScene::ResultScene()
{
	GameManager::SetSceneTag("ResultScene");

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
	if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN))
	{
		GameManager::SetScene(new TitleScene());
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
