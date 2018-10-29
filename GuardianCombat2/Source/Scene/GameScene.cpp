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

GameScene::GameScene()
{
	DirectionalLight* light = Object::Create<DirectionalLight>();		//環境光作成
	light->SetPause(true);											
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);							//GameManagerにライトを設定
	XModel* dome = Object::Create<XModel>();							//SkyDome作成
	dome->SetPause(true);
	dome->SetModelType(XModel::MODEL_DOME303);
	dome->SetScale(10.0f, 10.0f, 10.0f);
	field_ = Object::Create<MeshField>();								//フィールド作成
	Object::Create<EnemyHige>();										//敵作成
	player_ = Object::Create<FPSPlayer>();								//プレイヤー作成
	UI* ui = Object::Create<UI>(TextureManager::Tex_Mission);			//UI作成
	ui->SetScale(200.0f,40.0f,0.0f);
	ui->SetPosition(-200.0f, (float)ScreenHeight / 2.0f,0.0f);
	ui->MoveTexture(0.0f,5.0f,0, (float)ScreenHeight / 2.0f);
	ui->ScalingTexture(0.0f, 5.0f,200.0f,40.0f);
}

GameScene::~GameScene()
{
	//シャドウマップ終了処理
	ShadowMapShader::Uninit();
	//オブジェクト解放
	Object::ReleaseAll();
}

void GameScene::Init()
{
	
}

void GameScene::Uninit()
{
	
}

void GameScene::Update()
{
	//カーソルの位置固定
	SetCursorPos((int)ScreenWidth / 2,(int)ScreenHeight / 2);
	//オブジェクト更新
	Object::UpdateAll();
}

void GameScene::BeginDraw()
{
	ShadowMapShader::CreateShadowMapTexture();
	ShadowMapShader::CreateShadowMap();
	CRendererDirectX::Clear();
	Object::BeginDrawAll();
}

void GameScene::Draw()
{
	Object::DrawAll();
}

void GameScene::EndDraw()
{
	CRendererDirectX::ClearZ();
	Object::EndDrawAll();
}

Player * GameScene::GetPlayer()
{
	return player_;
}
