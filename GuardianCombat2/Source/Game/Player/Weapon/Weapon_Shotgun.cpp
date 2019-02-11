#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"
#include "WeaponState\WeaponRelative.h"
#include "../../Shader/OutlineShader.h"
#include "../../Shader/ShadowMapShader.h"
#include "../../Shader/ToonShader.h"
#include "Bullet\Bullet_Shotgun.h"
#include "../../../DInput/DirectInput.h"
#include "../../../XInput/xcontroller.h"
#include "../../../main.h"
#include "../../../Vector3/Vector3.h"
#include "../../../Sound/Sound.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//�e�̃��f���쐬
	model_ = new XModel();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
	relative_ = new WeaponRelative();

	//�V�F�[�_�[��K�p����B
	model_->SetUseShader(true);

	//�e����
	bullet_ = new Bullet_Shotgun[BulletNum];
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].SetWeapon(this);
		bullet_[i].Init();
	}

	shotSE_ = new Sound(SoundManager::BULLETSHOT_SE);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
	if (relative_)
	{
		delete relative_;
		relative_ = nullptr;
	}
	if (model_) {
		model_->Uninit();
		delete model_;
		model_ = nullptr;
	}
	if (shotSE_)
	{
		shotSE_->StopSound();
		delete shotSE_;
		shotSE_ = nullptr;
	}

	if (bullet_)
	{
		delete[] bullet_;
		bullet_ = nullptr;
	}
}

void Weapon_Shotgun::Init()
{
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Init();
		bullet_[i].SetVisible(false);
	}

	//���[���h�s���������
	D3DXMatrixIdentity(&world_);
	model_->SetWorld(world_);
	//���΍��W��������
	SetRelativePosition(-0.1f, 0.0f, 0.0f);
	//�X�P�[��������
	SetScale(0.05f,0.05f, 0.05f);
	//��]��񏉊���
	SetRotation(0,-90.0f,0);

	model_->Init();
}

void Weapon_Shotgun::Uninit()
{
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Uninit();
	}
	model_->Uninit();
}

void Weapon_Shotgun::Update()
{
	DIMOUSESTATE mouse = ReturnMouseMove();

	size_t rate_count = 0;
	if (fireRate_ > 0.0f) { 
		rate_count = (size_t)((float)GameFPS / fireRate_); }

	if (mouse.rgbButtons[0] == 0x80 || X_CONTROLLER::GetXcontrollerButtonPress(1, XINPUT_GAMEPAD_RIGHT_SHOULDER))
	{
		rateCount_++;
		if (rateCount_ >= rate_count)
		{
			shotSE_->PlaySoundA();
			ShotBullet(BulletNum);
			rateCount_ = 0;
		}
	}
	//���Ί֌W������s
	relative_->Act(this);

	model_->Update();

	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Update();
	}
}

void Weapon_Shotgun::BeginDraw()
{
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	model_->BeginDraw();


	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].BeginDraw();
	}

	//�V�F�[�_�[����
	if (model_->GetUseShader())
	{
		//�V���h�E�}�b�v�쐬
		LPD3DXEFFECT effect;
		effect = ShadowMapShader::GetEffect();
		ShadowMapShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("Shadow");
		model_->Draw(effect, 0);
	}
	
}

void Weapon_Shotgun::Draw()
{
	if (model_->GetUseShader())
	{
		LPD3DXEFFECT effect;

		//�A�E�g���C���`��
		effect = OutlineShader::GetEffect();
		OutlineShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("Tech");
		model_->Draw(effect, 0);

		//�g�D�[���V�F�[�_�[�`��
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(model_->GetWorld());
		effect->SetTechnique("ToonPaintInk");
		model_->Draw(effect, 0);
	}
	else
	{
		model_->Draw();
	}
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].Draw();
	}
}

void Weapon_Shotgun::EndDraw()
{
	model_->EndDraw();
	for (size_t i = 0; i < BulletNum; i++)
	{
		bullet_[i].EndDraw();
	}
}
