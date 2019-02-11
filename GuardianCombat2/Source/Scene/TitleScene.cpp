#include "TitleScene.h"
#include "../DInput/DirectInput.h"
#include "../Game/GameManager/GameManager.h"
#include "GameScene.h"
#include "EditScene.h"
#include "Fade.h"
#include "../Game/Object.h"
#include "../XInput/xcontroller.h"
#include "../Game/XModel/XModel.h"
#include "../Camera/StayCamera.h"
#include "../Light/Light.h"
#include "../Effekseer/Effekseer.h"
#include "../Texture/Texture.h"	
#include "../main.h"
#include "../Sound/Sound.h"
#include "Title\TitleSelect.h"

TitleScene::TitleScene()
{
	//SceneTagÇê›íË
	GameManager::SetSceneTag("TitleScene");
	//FadeOut
	GameManager::GetFade()->FadeOut();

	DirectionalLight* light = Object::Create<DirectionalLight>();		//ä¬ã´åıçÏê¨
	light->SetPause(true);
	light->SetVector(D3DXVECTOR3(0.0f, -1.0f, 1.0f));
	GameManager::SetDirectionalLight(light);										//GameManagerÇ…ÉâÉCÉgÇê›íË

	Camera* camera = Object::Create<StayCamera>();
	camera->SetPosition(0.0f, 0.0f, -10.0f);
	camera->SetAt(0.0f, 0.0f, 0.0f);
	camera->SetProjection();

	XModel* dome = Object::Create<XModel>();
	dome->SetModelType(XModel::MODEL_DOME336);
	dome->SetScale(0.01f, 0.01f, 0.01f);

	hige_ = Object::Create<XModel>();

	effect_ = new CEffekseer(CEffekseer::Effect_Teleport);

	Texture* logo = Object::Create<Texture>(TextureManager::Tex_TitleLogo);
	logo->SetDrawSize(logo->GetDrawSize().x * 1.5f, logo->GetDrawSize().y * 1.5f);

	bgm_ = new Sound(SoundManager::TITLE_BGM);
	selectOKSE_ = new Sound(SoundManager::SELECTOK_SE);

	titleSelect_ = new TitleSelect(this);
}

TitleScene::~TitleScene()
{
	if (effect_)
	{
		effect_->Uninit();
		delete effect_;
		effect_ = nullptr;
	}
	if (bgm_)
	{
		bgm_->StopSound();
		delete bgm_;
		bgm_ = nullptr;
	}
	if (selectOKSE_)
	{
		selectOKSE_->StopSound();
		delete selectOKSE_;
		selectOKSE_ = nullptr;
	}
	if (titleSelect_)
	{
		titleSelect_->Uninit();
		delete titleSelect_;
		titleSelect_ = nullptr;
	}
}

void TitleScene::Init()
{
	hige_->SetModelType(XModel::MODEL_HIGE);
	hige_->SetPosition(0.0f, 0.0f, 0.0f);
	hige_->SetRotation(0, 180.0f, 0);

	effect_->Init();
	effect_->SetScale(10.0f, 10.0f, 10.0f);
	effect_->RepeatEffect(true);
	effect_->SetVisible(true);

	bgm_->PlaySoundA();

	titleSelect_->Init();

	Object::InitAll();
}

void TitleScene::Uninit()
{
	bgm_->StopSound();
}

void TitleScene::Update()
{
	if (!sceneChange_)
	{
		titleSelect_->Update();

		if (GetKeyboardTrigger(DIK_RETURN) || GetKeyboardTrigger(DIK_SPACE) || 
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_A) || 
			X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_B))
		{
			SceneChange();
		}
	}
	else
	{
		if (ChangeSceneUpdate())
		{
			switch (titleSelect_->GetSelectMode())
			{
			case TitleSelect::EDIT:
				Object::ReleaseAll();
				GameManager::SetScene(new EditScene());
				break;
			case TitleSelect::GAME:
				Object::ReleaseAll();
				GameManager::SetScene(new GameScene());
				break;
			default:
				break;
			}

			return;
		}

	}
	if (up_) {
		value_ += 0.002f;
		if (value_ > 0.05f)
		{
			up_ = false;
		}
	}
	else {
		value_ -= 0.002f;
		if (value_ < -0.05f)
		{
			up_ = true;
		}
	}
	hige_->SetPositionY(hige_->GetPosition().y + value_);

	effect_->Update();
	Object::UpdateAll();
}

void TitleScene::BeginDraw()
{
	Object::BeginDrawAll();
}

void TitleScene::Draw()
{
	Object::DrawAll();
	effect_->Draw();
	titleSelect_->Draw();
}

void TitleScene::EndDraw()
{
	Object::EndDrawAll();
}

void TitleScene::SceneChange()
{
	Fade* fade = GameManager::GetFade();

	selectOKSE_->PlaySoundA();
	bgm_->StopSound();
	fade->FadeIn();
	sceneChange_ = true;
	if (titleSelect_->GetSelectMode() == (int)TitleSelect::EXIT)
	{
		Object::ReleaseAll();
		GameEnd();
	}
}
