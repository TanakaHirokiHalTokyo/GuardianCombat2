#include "GameScene.h"
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

GameScene::GameScene()
{
	//SceneTag設定
	GameManager::SetSceneTag("GameScene");
	GameManager::GameOver(false);
	GameManager::GetFade()->FadeOut();
	EffectManager::Init();
	pauseScene_ = new PauseScene();													//ポーズシーン作成

	bgm_ = new Sound(SoundManager::GAME_BGM);

}

GameScene::~GameScene()
{
	EffectManager::Uninit();

	//ポーズシーン破棄
	delete pauseScene_;
	pauseScene_ = nullptr;

	if (bgm_)
	{
		bgm_->StopSound();
		delete bgm_;
		bgm_ = nullptr;
	}

	//シャドウマップ終了処理
	ShadowMapShader::Uninit();
	//オブジェクト解放
	Object::ReleaseAll();
	Object::CollisionReleaseAll();
}

void GameScene::Init()
{
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
	player_->SetInvincible(false);

	EnemyHige* enemy = Object::Create<EnemyHige>();															//敵作成
	enemy->SetAutoAttack(true);
	enemy->SetInvincible(false);
	enemy->SetEditMode(false);

	UI* ui = Object::Create<UI>(TextureManager::Tex_Mission);			//UI作成
	ui->SetStartScale(200.0f, 40.0f);															//UI初期スケール設定
	ui->SetStartPosition(-200.0f, (float)ScreenHeight / 2.0f);					//UI初期座標設定
	ui->MoveTexture(0.0f, 5.0f, 0, (float)ScreenHeight / 2.0f);				//UI移動設定
	ui->ScalingTexture(0.0f, 5.0f, 200.0f, 40.0f);										//UI拡大設定
	Object::InitAll();
	pauseScene_->Init();
}

void GameScene::Uninit()
{
	pauseScene_->Uninit();
}

void GameScene::Update()
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
				GameManager::SetScene(new ResultScene());
				return;
			}
		}
	}
	else
	{
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

void GameScene::BeginDraw()
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
	player_camera->SetMainCameraInfo(player_camera->GetPosition(),player_camera->GetAt(),player_camera->GetUp(),player_camera->GetView(),player_camera->GetProj());
}

void GameScene::Draw()
{
	GameManager::GetBlur()->BeginDraw();
	Object::DrawAll();
	GameManager::GetBlur()->EndDraw();
	pauseScene_->Draw();
}

void GameScene::EndDraw()
{
	CRendererDirectX::ClearZ();
	Object::EndDrawAll();
}