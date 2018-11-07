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
#include "../Cube/Cube.h"

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

	//�g��U�����̃L���[�u�쐬
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];

	//�g��U���x�N�g���쐻
	circleShotParameter_.vec = new CircleVector[circleShotParameter_.CUBE_NUM];
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
	if (circleShotParameter_.cube)
	{
		delete[] circleShotParameter_.cube;
	}
	if (circleShotParameter_.vec)
	{
		delete[] circleShotParameter_.vec;
	}
	if (collision_)
	{
		delete collision_;
		collision_ = nullptr;
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

	//�g��U��������
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SetVisible(false);
		circleShotParameter_.cube[i].SetPosition(0.0f,0.0f,0.0f);
		circleShotParameter_.cube[i].SetScale(0.5f,0.5f,0.5f);
	}

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

	statePattern_->BeginDisplay();

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
		statePattern_->Display();
	}
}

void EnemyHige::EndDraw()
{
	statePattern_->EndDisplay();
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
EnemyHigeCircleShot::ENEMY_PARAMETER EnemyHige::GetCircleShotParameter()
{
	return circleShotParameter_;		//�g��U���擾
}
void EnemyHige::SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER parameter)
{
	circleShotParameter_ = parameter;	//�g��U���ݒ�
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;		//���X�g�{�b�N�X�̏����I��ԍ�
	static bool changeState = false;			//��Ԃ�ύX������
	static bool change_cube_num = false;		//�L���[�u�̐���ύX������
	static bool reset_position = false;			//�G�̃|�W�V�������Z�b�g�t���O

	//Window�ʒu�Œ�
	ImGui::SetNextWindowPos(ImVec2(10,(float)ScreenHeight / 2.0f));
	//�G�̃f�o�b�O���
	ImGui::Begin("Enemy Debug Info");
	//�G�̈ʒu�\��
	ImGui::Text("Position %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	reset_position = ImGui::Button("Reset Enemy Position");
	//�����݂�STATE��
	ImGui::Text("STATE : %s",StateWord[state_]);
	//���ɐݒ肷��STATE
	changeState = ImGui::ListBox("listbox\n(single select)", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);

	//�ҋ@��Ԃ̃p�����[�^�ݒ�
	if (ImGui::TreeNode("IDLE PARAMETER"))
	{
		ImGui::DragFloat("Speed Set", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);			//�X�s�[�h�ݒ�
		ImGui::DragFloat("RotateAngle", &idleParameter_.rot_angle, 0.01f, 0.01f, 6.0f);		//��]�X�s�[�h�ݒ�
		ImGui::TreePop();
	}
	//�ʏ�U���p�����[�^�ݒ�
	if (ImGui::CollapsingHeader("NORMAL ATTACK"))
	{
		//�ːi��Ԃ̃p�����[�^�ݒ�
		if (ImGui::TreeNode("RUSH PARAMETER"))
		{
			ImGui::DragFloat("Speed", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);			//�X�s�[�h�ݒ�
			ImGui::DragFloat("Length", &rushParameter_.length, 1.0f, 0.0f, 100.0f);			//�ːi�����ݒ�
			ImGui::TreePop();
		}
		//�g��U���̃p�����[�^�ݒ�
		if (ImGui::TreeNode("CIRCLESHOT PARAMETER"))
		{
			change_cube_num = ImGui::SliderInt("CUBE NUM", &circleShotParameter_.CUBE_NUM, 0, 20);				//�L���[�u���ݒ�
			ImGui::DragFloat("InitalVelocity", &circleShotParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);		//�������x�ݒ�
			ImGui::DragFloat("Acceleration", &circleShotParameter_.acceleration, 0.001f, 0.0f, 1.0f);			//�����x�ݒ�
			ImGui::DragFloat("Length", &circleShotParameter_.length, 1.0f, 0.0f, 100.0f);						//�L���[�u���΂������ݒ�
			ImGui::SliderFloat("CUBE SIZE", &circleShotParameter_.cubeSize, 0.1f, 1.0f);
			ImGui::TreePop();
		}
	}
	//Imugui�I��
	ImGui::End();

	//��ԕύX�����Ƃ�
	if (changeState)
	{
		//��ԕύX
		state_ = (STATE)listbox_item_current;
		changeState = false;
		statePattern_->ChangeState();
	}
	//�L���[�u�̐���ύX�����Ƃ�
	if (change_cube_num)
	{
		change_cube_num = false;

		//�L���[�u�̐����ύX����Ă�����
		if (circleShotParameter_.CUBE_NUM != circleShotParameter_.OLD_CUBE_NUM)
		{
			//���݂̃L���[�u�̐���ݒ�
			circleShotParameter_.OLD_CUBE_NUM = circleShotParameter_.CUBE_NUM;

			delete[] circleShotParameter_.cube;
			circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];

			delete[] circleShotParameter_.vec;
			circleShotParameter_.vec = new CircleVector[circleShotParameter_.CUBE_NUM];
		}
	}
	if (reset_position)
	{
		reset_position = false;
		SetPosition(0,0,0);
	}
	
}