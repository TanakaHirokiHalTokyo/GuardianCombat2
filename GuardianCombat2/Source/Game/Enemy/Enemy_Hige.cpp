#include "Enemy_Hige.h"
#include "../XModel/XModel.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../../Vector3/Vector3.h"
#include "../../Imgui/ImguiManager.h"
#include "StateEnemy\StateEnemy_SlowlyRotate.h"
#include "StateEnemy\StateEnemy_MoveToPlayer.h"

EnemyHige::EnemyHige()
{
	//���f���쐬
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::XMODEL::MODEL_HIGE);
	model_->SetUseShader(true);

	//�x�N�g���쐬
	vector_ = new Vector3();

	//��]����쐬
	rotate_ = new StateEnemy_SlowlyRotate();
	//�v���C���[�Ɍ���������쐬
	moveToPlayer_ = new StateEnemy_MoveToPlayer();
}

EnemyHige::~EnemyHige()
{
	delete vector_;
	delete rotate_;	
	delete moveToPlayer_;
}

void EnemyHige::Init()
{
	//������
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0,0,-8.0f);
	SetRotation(0,0,0);
	SetSpeed(0.02f);

	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	vector_->SetFront(0,0,1.0f);
	vector_->SetRight(1.0f,0.0f,0.0f);
	vector_->SetUp(0,1.0f,0);
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	//��]������s
	rotate_->Act(this);
	//�v���C���[�Ɍ�����������s
	moveToPlayer_->Act(this);

	model_->SetRotation(GetRotate());
	model_->SetPosition(GetPosition());
	model_->SetScale(GetScale());
}

void EnemyHige::BeginDraw()
{
	//���[���h�s��v�Z
	D3DXMATRIX trans, rotX, rotY, rotZ, scale;
	D3DXMatrixTranslation(&trans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixRotationX(&rotX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationY(&rotY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationZ(&rotZ, D3DXToRadian(transform_.rotate.z));
	D3DXMatrixScaling(&scale, transform_.scale.x, transform_.scale.y, transform_.scale.z);

	D3DXMatrixIdentity(&world_);

	world_ = scale;
	world_ *= rotX;
	world_ *= rotY;
	world_ *= rotZ;
	world_ *= trans;

	SetWorld(&world_);

	//�V�F�[�_�[����
	if (model_->GetUseShader())
	{	
		//�V���h�E�}�b�v�쐬
		LPD3DXEFFECT effect;
		effect = ShadowMapShader::GetEffect();
		ShadowMapShader::SetInfo(world_);
		effect->SetTechnique("Shadow");
		model_->Draw(effect, 0);
	}
}

void EnemyHige::Draw()
{
	if (model_->GetUseShader())
	{
		LPD3DXEFFECT effect;

		//�A�E�g���C���`��
		effect = OutlineShader::GetEffect();
		OutlineShader::SetInfo(world_);
		effect->SetTechnique("Tech");
		model_->Draw(effect, 0);

		//�g�D�[���V�F�[�_�[�`��
		effect = ToonShader::GetEffect();
		ToonShader::SetInfo(world_);
		effect->SetTechnique("ToonPaintInk");
		model_->Draw(effect, 0);
	}
}

void EnemyHige::EndDraw()
{
}
