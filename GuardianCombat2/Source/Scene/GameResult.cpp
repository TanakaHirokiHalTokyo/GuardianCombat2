#include "GameResult.h"
#include "../Texture/Texture.h"
#include "Fade.h"
#include "../DInput/DirectInput.h"
#include "../XInput/xcontroller.h"
#include "ResultScene.h"
#include "../Imgui/ImguiManager.h"
#include "../main.h"
#include "../Sound/Sound.h"
#include "../Camera/StayCamera.h"
#include "../Game/XModel/XModel.h"
#include "../Game/MeshField/MeshField.h"
#include "../Game/Enemy/RandomMoveHige.h"
#include "../Light/Light.h"
#include "../DirectXRenderer.h"
#include "../Game/Shader/ShadowMapShader.h"
#include <fstream>

const float WORD_SIZE_WIDTH = 600.0f * ((float)ScreenWidth / 1600.0f);				//�f�t�H���g���[�h�T�C�Y��
const float WORD_SIZE_HEGIHT = 200.0f * ((float)ScreenHeight / 900.0f);			//�f�t�H���g���[�h�T�C�Y����
const float WORD_MAXRATE = 2.0f;																			//���[�h�ő�l����
const float WORD_ADD_SPEED = 0.01f;																	//���[�h�g��X�s�[�h
const float BACK_SIZE_WIDTH = 1600.0f * ((float)ScreenWidth / 1600.0f);			//�w�i�T�C�Y��
const float BACK_SIZE_HEIGHT = 900.0f * ((float)ScreenHeight / 900.0f);				//�w�i�T�C�Y����
const float KEY_SIZE_WIDTH = 600.0f * ((float)ScreenWidth / 1600.0f);					//PressKey�T�C�Y��
const float KEY_SIZE_HEIGHT = 200.0f * ((float)ScreenHeight / 900.0f);				//PressKey�T�C�Y����
const int BACK_ALPHA = 0;		//�w�i�e�N�X�`���̓����x	

const char* RESULT_WORD[2] =
{
	"Win",
	"Lose"
};

GameResult::GameResult()
{
	this->LoadSettings("GameResultSettings");
	GameManager::GetFade()->FadeOut();
	bgm_ = new Sound(SoundManager::GAMERESULT_BGM);
	selectSE_ = new Sound(SoundManager::SELECTOK_SE);

	//�����쐬
	DirectionalLight* light = Object::Create<DirectionalLight>();		
	light->SetPause(true);
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);							

	//SkyDome�쐬
	XModel* skydome = Object::Create<XModel>();
	skydome->SetModelType(XModel::MODEL_DOME303);
	skydome->SetScale(10.0f,10.0f,10.0f);

	Object::Create<MeshField>();

	//�����_���ɓ����G�𐶐�
	enemy_ = Object::Create<RandomMoveHige>();
	enemy_->SetRangeX(-7.0f, 7.0f);
	enemy_->SetRangeY(0.0f,1.0f);
	enemy_->SetRangeZ(-7.0f, 7.0f);

	//�J�����̍쐬
	camera_ = Object::Create<StayCamera>();

	resultBack_ = Object::Create<Texture>(TextureManager::Tex_GameResultBack);
	pressKey_ = Object::Create<Texture>(TextureManager::Tex_PressSpace);

	//���s���ʂɂ���ăe�N�X�`���ؑ�
	if (GameManager::IsGameClear()) { resultWord_ = Object::Create<Texture>(TextureManager::Tex_Win);	selectResultNumber_ = 0; }
	else { resultWord_ = Object::Create<Texture>(TextureManager::Tex_Lose);		selectResultNumber_ = 1; }

}

GameResult::~GameResult()
{
	if (bgm_)
	{
		bgm_->StopSound();
		SAFE_DELETE(bgm_);
	}
	if (selectSE_)
	{
		selectSE_->StopSound();
		SAFE_DELETE(selectSE_);
	}
}

void GameResult::Init()
{
	bgm_->PlaySoundA();
	scaleAddRate_ = WORD_ADD_SPEED;
	resultWord_->SetDrawSize(WORD_SIZE_WIDTH,WORD_SIZE_HEGIHT);
	resultBack_->SetDrawSize(BACK_SIZE_WIDTH,BACK_SIZE_HEIGHT);
	pressKey_->SetDrawSize(KEY_SIZE_WIDTH,KEY_SIZE_HEIGHT);
	pressKey_->SetPosition(((float)ScreenWidth - KEY_SIZE_WIDTH) / 2.0f, (float)ScreenHeight - KEY_SIZE_HEIGHT, 0.0f);

	camera_->SetAt(0.0f, 0.0f, 0.0f);
	camera_->SetPosition(11.0f, 5.0f, 11.0f);

	Object::InitAll();
}

void GameResult::Uninit()
{
	GameManager::EndGame();
}

void GameResult::Update()
{
	//���ʕ\����ʕҏW
	if (GameManager::GetEnableEdit())
	{
		ImGui::Begin(u8"�Q�[�����U���g�ҏW");
		if (ImGui::Button(u8"�ۑ�"))
		{
			this->SaveSettings("GameResultSettings");
		}
		if (ImGui::ListBox(u8"���ʑI��", &selectResultNumber_, RESULT_WORD, IM_ARRAYSIZE(RESULT_WORD), 2))
		{
			switch (selectResultNumber_)
			{
			case 0:			//Win
				resultWord_->SetTexture(TextureManager::Tex_Win);
				break;
			case 1:			//Lose
				resultWord_->SetTexture(TextureManager::Tex_Lose);
				break;
			default:
				break;
			}
		}
		//�����F�ݒ�
		if (selectResultNumber_ == 0)	ImGui::ColorEdit3(u8"���������F",wordWinColor_);
		else ImGui::ColorEdit3(u8"�s�k�����F",wordLoseColor_)	;
		//�w�i�F�ݒ�
		if (selectResultNumber_ == 0)		ImGui::ColorEdit3(u8"�����w�i�F",backWinColor_);
		else	ImGui::ColorEdit3(u8"�����w�i�F", backLoseColor_);
		//PressKey�ݒ�
		ImGui::ColorEdit3(u8"PressKey�����F",keyColor_);
		ImGui::SliderFloat(u8"�_�ŃX�s�[�h",&this->alphaAddRate_,0.1f,10.0f);
		ImGui::End();
	}

	//�����F�X�V
	if (selectResultNumber_ == 0)		resultWord_->SetColor(D3DCOLOR_RGBA((int)(wordWinColor_[0] * 255.0f), (int)(wordWinColor_[1] * 255.0f), (int)(wordWinColor_[2] * 255.0f), 255));
	else	resultWord_->SetColor(D3DCOLOR_RGBA((int)(wordLoseColor_[0] * 255.0f), (int)(wordLoseColor_[1] * 255.0f), (int)(wordLoseColor_[2] * 255.0f), 255));
	//�w�i�F�X�V
	if (selectResultNumber_ == 0)	resultBack_->SetColor(D3DCOLOR_RGBA((int)(backWinColor_[0] * 255.0f), (int)(backWinColor_[1] * 255.0f), (int)(backWinColor_[2] * 255.0f), BACK_ALPHA));
	else	resultBack_->SetColor(D3DCOLOR_RGBA((int)(backLoseColor_[0] * 255.0f), (int)(backLoseColor_[1] * 255.0f), (int)(backLoseColor_[2] * 255.0f), BACK_ALPHA));

	//�g�嗦�X�V
	if (scaleAddRate_ > 0.0f)
	{
		scaleRate_ += scaleAddRate_;
		if (scaleRate_ >= WORD_MAXRATE) scaleAddRate_ *= -1.0f;
	}
	else
	{
		scaleRate_ += scaleAddRate_;
		if (scaleRate_ <= 1.0f) scaleAddRate_ *= -1.0f;
	}
	//���l�X�V
	if (alphaAddRate_ > 0.0f)
	{
		if (keyAlpha_ >= 255.0f) {
			alphaAddRate_ *= -1.0f;
			keyAlpha_ = 255.0f;
		}
		keyAlpha_ += alphaAddRate_;
	}
	else
	{
		if (keyAlpha_ <= 0.0f) {
			alphaAddRate_ *= -1.0f;
			keyAlpha_ = 0.0f;
		}
		keyAlpha_ += alphaAddRate_;
	}

	resultWord_->SetDrawSize(WORD_SIZE_WIDTH * scaleRate_, WORD_SIZE_HEGIHT * scaleRate_);
	resultWord_->SetPosition(((float)ScreenWidth - resultWord_->GetDrawSize().x) / 2.0f,((float)ScreenHeight - resultWord_->GetDrawSize().y) / 2.0f,0.0f);

	resultBack_->SetDrawSize(BACK_SIZE_WIDTH * scaleRate_, BACK_SIZE_HEIGHT * scaleRate_);
	resultBack_->SetPosition(((float)ScreenWidth - resultBack_->GetDrawSize().x) / 2.0f, ((float)ScreenHeight - resultBack_->GetDrawSize().y) / 2.0f, 0.0f);

	//Press�����F�X�V
	pressKey_->SetColor(D3DCOLOR_RGBA((int)(keyColor_[0] * 255.0f), (int)(keyColor_[1] * 255.0f), (int)(keyColor_[2] * 255.0f), (int)keyAlpha_));

	//Fade�|�C���^�擾
	Fade* fade = GameManager::GetFade();
	//�V�[���`�F���W
	if (!sceneChange_)
	{
		if (GetKeyboardTrigger(DIK_SPACE) || GetKeyboardTrigger(DIK_RETURN))
		{
			fade->FadeIn();
			selectSE_->PlaySoundA();
			sceneChange_ = true;
		}
	}
	else
	{
		if (ChangeSceneUpdate())
		{
			//�I�u�W�F�N�g���
			Object::ReleaseAll();
			GameManager::SetScene(new ResultScene());
			return;
		}
	}

	cameraAngle_ += 0.001f;
	if (cameraAngle_ >= 360.0f) cameraAngle_ = 0.0f;
	camera_->SetPosition(cosf(cameraAngle_) * 11.0f, 5.0f,sinf(cameraAngle_) * 11.0f);

	camera_->SetAt(enemy_->GetPosition().x, enemy_->GetPosition().y - 2.0f, enemy_->GetPosition().z);

	Object::UpdateAll();
}

void GameResult::BeginDraw()
{
	ShadowMapShader::CreateShadowMapTexture();
	ShadowMapShader::CreateShadowMap();
	CRendererDirectX::Clear();
	Object::BeginDrawAll();
}

void GameResult::Draw()
{
	Object::DrawAll();
}

void GameResult::EndDraw()
{
	Object::EndDrawAll();
	CRendererDirectX::ClearZ();
}

void GameResult::SaveSettings(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::out);
	file.write((const char*)&this->wordWinColor_, sizeof(this->wordWinColor_));
	file.write((const char*)&this->wordLoseColor_, sizeof(this->wordLoseColor_));
	file.write((const char*)&this->backWinColor_,sizeof(this->backWinColor_));
	file.write((const char*)&this->backLoseColor_, sizeof(this->backLoseColor_));
	file.write((const char*)&this->keyColor_, sizeof(this->keyColor_));
	file.close();
}

void GameResult::LoadSettings(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".gc2", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�Q�[�����U���g�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->wordWinColor_, sizeof(this->wordWinColor_));
		file.read((char*)&this->wordLoseColor_, sizeof(this->wordLoseColor_));
		file.read((char*)&this->backWinColor_, sizeof(this->backWinColor_));
		file.read((char*)&this->backLoseColor_, sizeof(this->backLoseColor_));
		file.read((char*)&this->keyColor_, sizeof(this->keyColor_));
	}
	file.close();
}
