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

GameScene::GameScene()
{
	//SceneTag設定
	GameManager::SetSceneTag("GameScene");

	pauseScene_ = new PauseScene();		//ポーズシーン作成

	if (!GameManager::GetGameObjectLoad())
	{
		DirectionalLight* light = Object::Create<DirectionalLight>();		//環境光作成
		light->SetPause(true);
		light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
		GameManager::SetDirectionalLight(light);							//GameManagerにライトを設定

		XModel* dome = Object::Create<XModel>();							//SkyDome作成
		dome->SetPause(true);
		dome->SetModelType(XModel::MODEL_DOME303);
		dome->SetScale(10.0f, 10.0f, 10.0f);

		Object::Create<MeshField>();										//フィールド作成

		player_ = GameManager::SetPlayer(Object::Create<FPSPlayer>());		//プレイヤー作成しマネージャーにプレイヤー登録する

		Object::Create<EnemyHige>();										//敵作成


		UI* ui = Object::Create<UI>(TextureManager::Tex_Mission);			//UI作成
		ui->SetStartScale(200.0f,40.0f);									//UI初期スケール設定
		ui->SetStartPosition(-200.0f, (float)ScreenHeight / 2.0f);			//UI初期座標設定
		ui->MoveTexture(0.0f, 5.0f, 0, (float)ScreenHeight / 2.0f);			//UI移動設定
		ui->ScalingTexture(0.0f, 5.0f, 200.0f, 40.0f);						//UI拡大設定

		cube_ = Object::Create<Cube>();
		

		GameManager::SetGameObjectLoad(true);								//GameObjectを読み込んだ設定
	}
}

GameScene::~GameScene()
{
	//ポーズシーン破棄
	delete pauseScene_;
	pauseScene_ = nullptr;

	//シャドウマップ終了処理
	ShadowMapShader::Uninit();
	//オブジェクト解放
	Object::ReleaseAll();
}

void GameScene::Init()
{
	Object::InitAll();
	pauseScene_->Init();
}

void GameScene::Uninit()
{
	pauseScene_->Uninit();
}

void GameScene::Update()
{
	//ポーズシーンの更新
	pauseScene_->Update();

	if (!pauseScene_->GetPause())
	{
		//カーソルの位置固定
		SetCursorPos((int)ScreenWidth / 2, (int)ScreenHeight / 2);
		//オブジェクト更新
		Object::UpdateAll();

	}
	
	if (GetKeyboardTrigger(DIK_SPACE))
	{
		GameManager::SetScene(new ResultScene());
	}
}

void GameScene::BeginDraw()
{
	ShadowMapShader::CreateShadowMapTexture();
	ShadowMapShader::CreateShadowMap();
	CRendererDirectX::Clear();
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
	Object::DrawAll();
	pauseScene_->Draw();
}

void GameScene::EndDraw()
{
	CRendererDirectX::ClearZ();
	Object::EndDrawAll();
}