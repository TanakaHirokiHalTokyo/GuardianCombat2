#include "Enemy_Hige.h"
#include "../XModel/XModel.h"
#include "../Shader/ToonShader.h"
#include "../Shader/OutlineShader.h"
#include "../Shader/ShadowMapShader.h"
#include "../../Vector3/Vector3.h"
#include "../../Imgui/ImguiManager.h"
#include "StateEnemy\StatePattern_EnemyHige.h"
#include "../../Collision/Collision.h"
#include "../../Debug/Debug.h"
#include "../../main.h"

EnemyHige::EnemyHige()
{
	//���f���쐬
	model_ = Object::Create<XModel>();
	model_->SetModelType(XModel::XMODEL::MODEL_HIGE);
	model_->SetUseShader(true);

	//�x�N�g���쐬
	vector_ = new Vector3();

	//�X�e�[�g�Ǘ�
	statePattern_ = new StatePattern_EnemyHige(this);

	//�R���W�����쐬
	collision_ = new Sphere();
	collision_->object_ = this;
	collision_->pos = GetPosition();

	//�R���W���������쐬
	debugCollision_ = Object::Create<DebugSphere>();

}

EnemyHige::~EnemyHige()
{
	if (vector_)
	{
		delete vector_;
		vector_ = nullptr;
	}
	
	if (statePattern_ )
	{
		delete statePattern_;
		statePattern_ = nullptr;
	}
}

void EnemyHige::Init()
{
	//������
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(0,0,-8.0f);
	SetRotation(0,0,0);
	SetSpeed(0.02f);

	//���f����񏉊���
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	//�x�N�g��������
	vector_->SetFront(0,0,1.0f);
	vector_->SetRight(1.0f,0.0f,0.0f);
	vector_->SetUp(0,1.0f,0);

	//Collision������
	collision_->rad = 0.5f;

	//�f�o�b�O�\���̃��f�B�E�X�ݒ�
	debugCollision_->SetRadius(collision_->rad);

	//�f�o�b�O���[�hON
	debug_ = true;
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	//�X�e�[�g������s
	statePattern_->Act();

	//���f�����X�V
	model_->SetRotation(GetRotate());
	model_->SetPosition(GetPosition());
	model_->SetScale(GetScale());

	//�R���W�����X�V
	collision_->pos = GetPosition();
	debugCollision_->SetPosition(GetPosition());
	debugCollision_->SetPositionY(debugCollision_->GetRadius());
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

	if (debug_)
	{
		//�f�o�b�O�\���@�X�e�[�g�E�p�����[�^�\��
		DrawDebug();
	}
}

void EnemyHige::EndDraw()
{
	
}

EnemyHige::STATE EnemyHige::GetState()
{
	return state_;		//��Ԏ擾
}

void EnemyHige::SetState(STATE state)
{
	state_ = state;		//��Ԑݒ�
}
void EnemyHige::FinishState()
{
	state_ = IDLE;
	statePattern_->ChangeState();
}
EnemyHigeRush::ENEMY_PARAMETER EnemyHige::GetRushParameter()
{
	return rushParameter_;		//�ːi��Ԃ̃p�����[�^�擾
}
void EnemyHige::SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER parameter)
{
	rushParameter_ = parameter;	//�ːi��Ԃ̃p�����[�^�ݒ�
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;
	static bool changeState = false;

	//Window�ʒu�Œ�
	ImGui::SetNextWindowPos(ImVec2(10,(float)ScreenHeight / 2.0f));
	//�G�̃f�o�b�O���
	ImGui::Begin("Enemy Debug Info");
	//�����݂�STATE��
	ImGui::Text("STATE : %s",StateWord[state_]);
	//���ɐݒ肷��STATE
	changeState = ImGui::ListBox("listbox\n(single select)", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);

	//�ҋ@��Ԃ̃p�����[�^�ݒ�
	if (ImGui::TreeNode("IDLE PARAMETER"))
	{
		ImGui::DragFloat("Speed Set", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);
		ImGui::DragFloat("RotateAngle", &idleParameter_.rot_angle, 0.1f, 0.0f, 10.0f);
		ImGui::TreePop();
	}
	//�ʏ�U���p�����[�^�ݒ�
	if (ImGui::CollapsingHeader("NORMAL ATTACK"))
	{
		//�ːi��Ԃ̃p�����[�^�ݒ�
		if (ImGui::TreeNode("RUSH PARAMETER"))
		{
			ImGui::DragFloat("Speed Set", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Length Set", &rushParameter_.length, 1.0f, 0.0f, 100.0f);
			ImGui::TreePop();
		}
	}
	//Imugui�I��
	ImGui::End();

	if (changeState)
	{
		//��ԕύX
		state_ = (STATE)listbox_item_current;
		changeState = false;
		statePattern_->ChangeState();
	}

	statePattern_->Display();
}