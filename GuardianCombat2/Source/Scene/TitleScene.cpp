#include "TitleScene.h"
#include "../DInput/DirectInput.h"
#include "../Game/GameManager/GameManager.h"
#include "GameScene.h"
#include "../Game/Object.h"

TitleScene::TitleScene()
{
	//SceneTagÇê›íË
	GameManager::SetSceneTag("TitleScene");
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
	if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE))
	{
		GameManager::SetScene(new GameScene());
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
