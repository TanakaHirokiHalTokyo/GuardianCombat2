#include "EditScene.h"
#include "../Game/Object.h"
#include "../Game/Player/PlayerCamera.h"
#include "../Light/Light.h"
#include "../Game/XModel/XModel.h"
#include "../Game/Player/Player.h"
#include "../Game/SkinMesh/sceneSkinMesh.h"
#include "../Game/GameManager/GameManager.h"
#include "../Game/MeshField/MeshField.h"	
#include "../Game/Shader/ShadowMapShader.h"
#include "../DirectXRenderer.h"
#include "../Game/Enemy/Enemy_Hige.h"
#include "../Game/Player/FPSPlayer.h"
#include "../UI/UI.h"
#include "Fade.h"
#include "../DInput/DirectInput.h"
#include "ResultScene.h"
#include "PauseScene.h"
#include "../Game/Cube/Cube.h"	
#include "../Game/Effect/Effect.h"
#include "../Game/Blur/Blur.h"
#include "../Sound/Sound.h"
#include "TitleScene.h"
#include "GameResult.h"

EditScene::EditScene()
{
	//SceneTag設定
	Object::SetDrawJudge(false);
	GameManager::SetSceneTag("EditScene");
	GameManager::ResetEnding();
	GameManager::SetGameClear(false);
	GameManager::GameOver(false);
	GameManager::GetFade()->FadeOut();
	EffectManager::Init();

	pauseScene_ = new PauseScene();													//ポーズシーン作成
	pauseScene_->SetEnableClick(false);

	bgm_ = new Sound(SoundManager::GAME_BGM);
	bgm_->PlaySoundA();

	DirectionalLight* light = Object::Create<DirectionalLight>();		//環境光作成
	light->SetPause(true);
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);										//GameManagerにライトを設定

	XModel* dome = Object::Create<XModel>();								//SkyDome作成
	dome->SetPause(true);
	dome->SetModelType(XModel::MODEL_DOME303);
	dome->SetScale(10.0f, 10.0f, 10.0f);

	Object::Create<MeshField>();														//フィールド作成

	player_ = GameManager::SetPlayer(Object::Create<FPSPlayer>());		//プレイヤー作成しマネージャーにプレイヤー登録する
	EnemyHige* enemy = Object::Create<EnemyHige>();								//敵作成
	enemy->SetAutoAttack(false);
	enemy->SetInvincible(true);
	enemy->SetEditMode(true);
}

EditScene::~EditScene()
{
	EffectManager::Uninit();

	//ポーズシーン破棄
	if (pauseScene_)
	{
		delete pauseScene_;
		pauseScene_ = nullptr;
	}

	if (bgm_)
	{
		bgm_->StopSound();
		delete bgm_;
		bgm_ = nullptr;
	}

	//シャドウマップ終了処理
	ShadowMapShader::Uninit();
	
}

void EditScene::Init()
{
	player_->SetInvincible(true);
	Object::InitAll();
	pauseScene_->Init();
}

void EditScene::Uninit()
{
	pauseScene_->Uninit();
}

void EditScene::Update()
{
	//Fadeポインタ取得
	Fade* fade = GameManager::GetFade();

	if (GameManager::GetGameOver())
	{
		//シーンチェンジ
		if (!sceneChange_)
		{
			fade->FadeIn();
			sceneChange_ = true;
		}
		else
		{
			if (ChangeSceneUpdate())
			{
				//オブジェクト解放
				Object::ReleaseAll();
				Object::CollisionReleaseAll();
				GameManager::SetScene(new GameResult());
				return;
			}
		}
	}
	else
	{
		if (GameManager::GetReturnTitle())
		{
			if (!sceneChange_)
			{
				fade->FadeIn();
				sceneChange_ = true;
			}
			else
			{
				if (ChangeSceneUpdate())
				{
					//オブジェクト解放
					Object::ReleaseAll();
					Object::CollisionReleaseAll();
					GameManager::SetScene(new TitleScene());
					GameManager::SetReturnTitle(false);
					return;
				}
			}
		}

		//ポーズシーンの更新
		pauseScene_->Update();

		if (!pauseScene_->GetPause())
		{
			if (GetKeyboardTrigger(DIK_F8))
			{
				cursorActive_ = !cursorActive_;
			}

			if (cursorActive_)
			{
				//カーソルの位置固定
				SetCursorPos((int)ScreenWidth / 2, (int)ScreenHeight / 2);
			}
			//オブジェクト更新
			Object::UpdateAll();
		}
	}

}

void EditScene::BeginDraw()
{
	ShadowMapShader::CreateShadowMapTexture();
	ShadowMapShader::CreateShadowMap();
	CRendererDirectX::Clear();

	Object::JudgementAll();
	Object::BeginDrawAll();

	if (player_ == nullptr)
	{
		player_ = GameManager::GetPlayer();
	}

	Camera* player_camera = player_->GetCamera();
	player_camera->SetMainCameraInfo(player_camera->GetPosition(), player_camera->GetAt(), player_camera->GetUp(), player_camera->GetView(), player_camera->GetProj());
}

void EditScene::Draw()
{
	GameManager::GetBlur()->BeginDraw();
	Object::DrawAll();
	GameManager::GetBlur()->EndDraw();
}

void EditScene::EndDraw()
{
	Object::EndDrawAll();
	pauseScene_->Draw();
	CRendererDirectX::ClearZ();
}