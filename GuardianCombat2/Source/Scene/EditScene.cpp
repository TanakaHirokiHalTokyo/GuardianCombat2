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

EditScene::EditScene()
{
	//SceneTag�ݒ�
	GameManager::SetSceneTag("EditScene");
	GameManager::GameOver(false);
	GameManager::GetFade()->FadeOut();
	EffectManager::Init();

	pauseScene_ = new PauseScene();													//�|�[�Y�V�[���쐬

	
	bgm_ = new Sound(SoundManager::GAME_BGM);
}

EditScene::~EditScene()
{
	EffectManager::Uninit();

	//�|�[�Y�V�[���j��
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

	//�V���h�E�}�b�v�I������
	ShadowMapShader::Uninit();
	//�I�u�W�F�N�g���
	Object::ReleaseAll();
	Object::CollisionReleaseAll();
}

void EditScene::Init()
{
	bgm_->PlaySoundA();

	DirectionalLight* light = Object::Create<DirectionalLight>();		//�����쐬
	light->SetPause(true);
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);										//GameManager�Ƀ��C�g��ݒ�

	XModel* dome = Object::Create<XModel>();								//SkyDome�쐬
	dome->SetPause(true);
	dome->SetModelType(XModel::MODEL_DOME303);
	dome->SetScale(10.0f, 10.0f, 10.0f);

	Object::Create<MeshField>();														//�t�B�[���h�쐬

	player_ = GameManager::SetPlayer(Object::Create<FPSPlayer>());		//�v���C���[�쐬���}�l�[�W���[�Ƀv���C���[�o�^����
	player_->SetInvincible(true);

	EnemyHige* enemy = Object::Create<EnemyHige>();															//�G�쐬
	enemy->SetAutoAttack(false);
	enemy->SetInvincible(true);
	enemy->SetEditMode(true);

	UI* ui = Object::Create<UI>(TextureManager::Tex_Mission);			//UI�쐬
	ui->SetStartScale(200.0f, 40.0f);															//UI�����X�P�[���ݒ�
	ui->SetStartPosition(-200.0f, (float)ScreenHeight / 2.0f);					//UI�������W�ݒ�
	ui->MoveTexture(0.0f, 5.0f, 0, (float)ScreenHeight / 2.0f);				//UI�ړ��ݒ�
	ui->ScalingTexture(0.0f, 5.0f, 200.0f, 40.0f);										//UI�g��ݒ�
	Object::InitAll();
	pauseScene_->Init();
}

void EditScene::Uninit()
{
	pauseScene_->Uninit();
}

void EditScene::Update()
{
	//Fade�|�C���^�擾
	Fade* fade = GameManager::GetFade();

	if (GameManager::GetGameOver())
	{
		//�V�[���`�F���W
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
		//�|�[�Y�V�[���̍X�V
		pauseScene_->Update();

		if (!pauseScene_->GetPause())
		{
			if (GetKeyboardTrigger(DIK_F8))
			{
				cursorActive_ = !cursorActive_;
			}

			if (cursorActive_)
			{
				//�J�[�\���̈ʒu�Œ�
				SetCursorPos((int)ScreenWidth / 2, (int)ScreenHeight / 2);
			}
			//�I�u�W�F�N�g�X�V
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
	pauseScene_->Draw();
}

void EditScene::EndDraw()
{
	CRendererDirectX::ClearZ();
	Object::EndDrawAll();
}