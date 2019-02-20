#include "../Game/Object.h"
#include "../Game/Player/PlayerCamera.h"
#include "../Light/Light.h"
#include "../Game/XModel/XModel.h"
#include "../Game/Player/Player.h"
#include "../Game/GameManager/GameManager.h"
#include "../Game/MeshField/MeshField.h"	
#include "../Game/Shader/ShadowMapShader.h"
#include "../DirectXRenderer.h"
#include "../Game/Enemy/Enemy_Hige.h"
#include "../Game/Player/FPSPlayer.h"
#include "../UI/UI.h"
#include "Fade.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"
#include "GameResult.h"
#include "PauseScene.h"
#include "../Game/Cube/Cube.h"	
#include "../Game/Effect/Effect.h"
#include "../Sound/Sound.h"
#include "../Imgui/ImguiManager.h"
#include "TitleScene.h"
#include "Tutorial.h"
#include "GameScene.h"

//�ۑ��E�ǂݍ��݁@UI�ݒ�t�@�C��
const std::string UI_SETTING_FILENAME = "TutorialUISettings";

Tutorial::Tutorial()
{
	LoadUISettings(UI_SETTING_FILENAME);
	//SceneTag�ݒ�
	Object::SetDrawJudge(false);
	GameManager::StartGame();
	GameManager::SetSceneTag("TutorialScene");
	GameManager::SetGameClear(false);
	GameManager::ResetEnding();
	GameManager::GameOver(false);
	GameManager::GetFade()->FadeOut();
	EffectManager::Init();
	pauseScene_ = new PauseScene();													//�|�[�Y�V�[���쐬

	bgm_ = new Sound(SoundManager::GAME_BGM);
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

	enemy_ = Object::Create<EnemyHige>();															//�G�쐬
	enemy_->SetAutoAttack(false);
	enemy_->SetInvincible(true);
	enemy_->SetEditMode(false);

	explainMouse_ = new Texture(TextureManager::Tex_TutorialMouseExplain);
	explainTrans_ = new Texture(TextureManager::Tex_TutorialTransExplain);
	tutorialWord_ = new Texture(TextureManager::Tex_TutorialWord);
	finishTutorial_ = new Texture(TextureManager::Tex_TutorialFinish);
}

Tutorial::~Tutorial()
{
	if (pauseScene_) pauseScene_->Uninit(); SAFE_DELETE(pauseScene_);
	if (bgm_) bgm_->StopSound(); SAFE_DELETE(bgm_);
	if (explainMouse_) explainMouse_->Uninit(); SAFE_DELETE(explainMouse_);
	if (explainTrans_) explainTrans_->Uninit(); SAFE_DELETE(explainTrans_);
	if (tutorialWord_) tutorialWord_->Uninit(); SAFE_DELETE(tutorialWord_);
	if (finishTutorial_) finishTutorial_->Uninit(); SAFE_DELETE(finishTutorial_);
}

void Tutorial::Init()
{
	pauseScene_->Init();

	explainMouse_->Init();
	explainTrans_->Init();
	tutorialWord_->Init();
	finishTutorial_->Init();

	explainTrans_->SetDrawSize(explainTransSize_[0], explainTransSize_[1]);
	explainMouse_->SetDrawSize(explainMouseSize_[0], explainMouseSize_[1]);
	tutorialWord_->SetDrawSize(tutorialWordSize_[0], tutorialWordSize_[1]);
	finishTutorial_->SetDrawSize(tutorialFinishSize_[0],tutorialFinishSize_[1]);

	explainMouse_->SetPosition(explainMousePos_[0], explainMousePos_[1], 0);
	explainTrans_->SetPosition(explainTransPos_[0], explainTransPos_[1], 0);
	tutorialWord_->SetPosition(tutorialWordPos_[0], tutorialWordPos_[1],0);
	finishTutorial_->SetPosition(tutorialFinishPos_[0],tutorialFinishPos_[1],0);

	Object::InitAll();
}

void Tutorial::Uninit()
{
	explainMouse_->Uninit();
	explainTrans_->Uninit();
	tutorialWord_->Uninit();
	finishTutorial_->Uninit();

	//�V���h�E�}�b�v�I������
	ShadowMapShader::Uninit();
}

void Tutorial::Update()
{
	Fade* fade = GameManager::GetFade();

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
				//�I�u�W�F�N�g���
				Object::ReleaseAll();
				Object::CollisionReleaseAll();
				GameManager::SetScene(new TitleScene());
				GameManager::SetReturnTitle(false);
				return;
			}
		}
	}
	if (GetKeyboardTrigger(DIK_K) || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_X))
	{
		playGame_ = true;
	}
	if (playGame_)
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
				//�I�u�W�F�N�g���
				Object::ReleaseAll();
				Object::CollisionReleaseAll();
				GameManager::SetScene(new GameScene());
				return;
			}
		}
	}

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

	UpdateUI();			//UI�X�V
}

void Tutorial::BeginDraw()
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

void Tutorial::Draw()
{
	Object::DrawAll();
}

void Tutorial::EndDraw()
{
	DrawUI();		//UI�`��
	Object::EndDrawAll();

	pauseScene_->Draw();
	CRendererDirectX::ClearZ();
}

void Tutorial::UpdateUI()
{
	if (GameManager::GetEnableEdit())
	{
		ImGui::Begin(u8"UI�ݒ�");
		if (ImGui::Button(u8"�ۑ�"))	this->SaveUISettings(UI_SETTING_FILENAME);
		ImGui::DragFloat2(u8"�`���[�g���A�������@���W", tutorialWordPos_, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�`���[�g���A�������@�T�C�Y", tutorialWordSize_, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�`���[�g���A���I�������@���W", tutorialFinishPos_, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�`���[�g���A���I�������@�T�C�Y", tutorialFinishSize_, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�ړ������@���W",explainTransPos_,1.0f,0.0f,(float)ScreenWidth);
		ImGui::DragFloat2(u8"�ړ������@�T�C�Y", explainTransSize_, 1.0f, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�}�E�X�����@���W", explainMousePos_, 0.0f, (float)ScreenWidth);
		ImGui::DragFloat2(u8"�}�E�X�����@�T�C�Y", explainMouseSize_, 0.0f, (float)ScreenWidth);
		ImGui::End();

		explainTrans_->SetDrawSize(explainTransSize_[0], explainTransSize_[1]);
		explainMouse_->SetDrawSize(explainMouseSize_[0], explainMouseSize_[1]);
		tutorialWord_->SetDrawSize(tutorialWordSize_[0], tutorialWordSize_[1]);
		finishTutorial_->SetDrawSize(tutorialFinishSize_[0],tutorialFinishSize_[1]);

		explainMouse_->SetPosition(explainMousePos_[0], explainMousePos_[1],0);
		explainTrans_->SetPosition(explainTransPos_[0], explainTransPos_[1],0);
		finishTutorial_->SetPosition(tutorialFinishPos_[0],tutorialFinishPos_[1],0);
		tutorialWord_->SetPosition(tutorialWordPos_[0], tutorialWordPos_[1], 0);
	}
	UpdateTutorialWord();

	explainMouse_->Update();
	explainTrans_->Update();
	finishTutorial_->Update();
	tutorialWord_->Update();
}

void Tutorial::DrawUI()
{
	explainMouse_->Draw();
	explainTrans_->Draw();
	finishTutorial_->Draw();
	tutorialWord_->Draw();
}

void Tutorial::UpdateTutorialWord()
{
	if (tutorialWordAlphaSpeed_ > 0.0f)
	{
		tutorialWordAlpha_ += tutorialWordAlphaSpeed_;		//���l���Z
		if (tutorialWordAlpha_ >= 255.0f) {
			tutorialWordAlpha_ = 255.0f;
			tutorialWordAlphaSpeed_ *= -1.0f;
		}
	}
	else
	{
		tutorialWordAlpha_ += tutorialWordAlphaSpeed_;			//���l���Z
		if (tutorialWordAlpha_ <= 0.0f) {
			tutorialWordAlpha_ = 0.0f;
			tutorialWordAlphaSpeed_ *= -1.0f;
		}
	}

	tutorialWord_->SetColor(D3DCOLOR_RGBA(255,255,255,(int)tutorialWordAlpha_));
}

void Tutorial::SaveUISettings(std::string filename)
{
	if (GameManager::GetEnableEdit())
	{
		std::ofstream file;
		file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::out);
		file.write((const char*)&this->explainMousePos_,sizeof(this->explainMousePos_));
		file.write((const char*)&this->explainMouseSize_, sizeof(this->explainMouseSize_));
		file.write((const char*)&this->explainTransPos_, sizeof(this->explainTransPos_));
		file.write((const char*)&this->explainTransSize_, sizeof(this->explainTransSize_));
		file.write((const char*)&this->tutorialWordPos_, sizeof(this->tutorialWordPos_));
		file.write((const char*)&this->tutorialWordSize_, sizeof(this->tutorialWordSize_));
		file.write((const char*)&this->tutorialFinishPos_, sizeof(this->tutorialFinishPos_));
		file.write((const char*)&this->tutorialFinishSize_, sizeof(this->tutorialFinishSize_));
		file.close();
	}
}

void Tutorial::LoadUISettings(std::string filename)
{
	if (GameManager::GetEnableEdit())
	{
		std::ifstream file;
		file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
		if (file.fail())
		{
			MessageBoxA(NULL, "TutorialUI�ݒ�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
		}
		else
		{
			file.read((char*)&this->explainMousePos_, sizeof(this->explainMousePos_));
			file.read((char*)&this->explainMouseSize_, sizeof(this->explainMouseSize_));
			file.read((char*)&this->explainTransPos_, sizeof(this->explainTransPos_));
			file.read((char*)&this->explainTransSize_, sizeof(this->explainTransSize_));
			file.read((char*)&this->tutorialWordPos_, sizeof(this->tutorialWordPos_));
			file.read((char*)&this->tutorialWordSize_, sizeof(this->tutorialWordSize_));
			file.read((char*)&this->tutorialFinishPos_, sizeof(this->tutorialFinishPos_));
			file.read((char*)&this->tutorialFinishSize_, sizeof(this->tutorialFinishSize_));
		}
		file.close();
	}
}