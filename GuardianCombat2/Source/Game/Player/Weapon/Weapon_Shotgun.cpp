#include "Weapon_Shotgun.h"
#include "../Player.h"
#include "../../../Camera/Camera.h"
#include "../../XModel/XModel.h"
#include "WeaponState\WeaponRelative.h"
#include "../../Shader/OutlineShader.h"
#include "../../Shader/ShadowMapShader.h"
#include "../../Shader/ToonShader.h"
#include "Bullet\Bullet_Shotgun.h"

Weapon_Shotgun::Weapon_Shotgun()
{
	//�e�̃��f���쐬
	model_ = new XModel();
	model_->SetModelType(XModel::XMODEL::MODEL_SHOTGUN);
	relative_ = new WeaponRelative();

	//�V�F�[�_�[��K�p����B
	model_->SetUseShader(true);
	//�e�q�֌W��K�p
	model_->SetHieral(false);

	//�e����
	bullet_ = Object::Create<Bullet_Shotgun>();
	bullet_->SetWeapon(this);
}

Weapon_Shotgun::~Weapon_Shotgun()
{
	delete relative_;
	delete model_;
}

void Weapon_Shotgun::Init()
{
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
	model_->Uninit();
}

void Weapon_Shotgun::Update()
{
	model_->Update();
}

void Weapon_Shotgun::BeginDraw()
{
	//���Ί֌W������s
	relative_->Act(this);

	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	model_->BeginDraw();

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
}

void Weapon_Shotgun::EndDraw()
{
	model_->EndDraw();
}
