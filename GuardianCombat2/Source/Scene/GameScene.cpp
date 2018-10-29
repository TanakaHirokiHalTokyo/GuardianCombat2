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
	DirectionalLight* light = Object::Create<DirectionalLight>();		//�����쐬
	light->SetPause(true);											
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);							//GameManager�Ƀ��C�g��ݒ�
	XModel* dome = Object::Create<XModel>();							//SkyDome�쐬
	dome->SetPause(true);
	dome->SetModelType(XModel::MODEL_DOME303);
	dome->SetScale(10.0f, 10.0f, 10.0f);
	field_ = Object::Create<MeshField>();								//�t�B�[���h�쐬
	Object::Create<EnemyHige>();										//�G�쐬
	player_ = Object::Create<FPSPlayer>();								//�v���C���[�쐬
	UI* ui = Object::Create<UI>(TextureManager::Tex_Mission);			//UI�쐬
	ui->SetScale(200.0f,40.0f,0.0f);
	ui->SetPosition(-200.0f, (float)ScreenHeight / 2.0f,0.0f);
	ui->MoveTexture(0.0f,5.0f,0, (float)ScreenHeight / 2.0f);
	ui->ScalingTexture(0.0f, 5.0f,200.0f,40.0f);
}

GameScene::~GameScene()
{
	//�V���h�E�}�b�v�I������
	ShadowMapShader::Uninit();
	//�I�u�W�F�N�g���
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
	//�J�[�\���̈ʒu�Œ�
	SetCursorPos((int)ScreenWidth / 2,(int)ScreenHeight / 2);
	//�I�u�W�F�N�g�X�V
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
