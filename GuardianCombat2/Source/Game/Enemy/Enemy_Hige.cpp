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

const D3DXVECTOR3 init_pos = D3DXVECTOR3(0,0,-8.0f);

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
	
	//�p�����[�^������
	InitParameter();
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
	if (collision_)
	{
		delete collision_;
		collision_ = nullptr;
	}

	DestParameter();
}

void EnemyHige::Init()
{
	//������
	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(init_pos);
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

	InitParameterValue();

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
	debugCollision_->SetPosition(GetPosition());
	debugCollision_->SetPositionY(debugCollision_->GetRadius());
	collision_->pos = debugCollision_->GetPosition();
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
void EnemyHige::SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER* parameter)
{
	rushParameter_ = *parameter;	//�ːi��Ԃ̃p�����[�^�ݒ�
}
EnemyHigeCircleShot::ENEMY_PARAMETER EnemyHige::GetCircleShotParameter()
{
	return circleShotParameter_;		//�g��U���擾
}
void EnemyHige::SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter)
{
	circleShotParameter_ = *parameter;	//�g��U���ݒ�
}
EnemyHigeHorming::ENEMY_PARAMETER EnemyHige::GetHormingParameter()
{
	return hormingParameter_;			//�z�[�~���O�p�����[�^�擾
}
void EnemyHige::SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER * parameter)
{
	hormingParameter_ = *parameter;		//�z�[�~���O�p�����[�^�ݒ�
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;		//���X�g�{�b�N�X�̏����I��ԍ�
	static bool changeState = false;			//��Ԃ�ύX������
	static bool change_cube_num = false;		//�L���[�u�̐���ύX������
	static bool reset_position = false;			//�G�̃|�W�V�������Z�b�g�t���O
	static bool reset_parameter = false;		//�p�����[�^���Z�b�g�t���O
	static bool change_hormingcube_num = false;	//�z�[�~���O�L���[�u�̐���ύX������

	//Window�ʒu�Œ�
	ImGui::SetNextWindowPos(ImVec2(10,(float)ScreenHeight / 2.0f));
	//�G�̃f�o�b�O���
	ImGui::Begin("Enemy Debug Info");
	//�G�̈ʒu�\��
	ImGui::Text("Position %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	//�|�W�V����������
	reset_position = ImGui::Button("Reset Enemy Position");
	//�p�����[�^������
	reset_parameter = ImGui::Button("Reset Parameter");
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
		//�z�[�~���O�̃p�����[�^�ݒ�
		if (ImGui::TreeNode("HORMING PARAMETER"))
		{
			change_hormingcube_num = ImGui::SliderInt("CUBE NUM", &hormingParameter_.CUBE_NUM, 0, 20);		//�L���[�u���ݒ�
			ImGui::SliderFloat("FanAngle",&hormingParameter_.fanangle,60.0f,180.0f,"%.1f",1.0f);			//��̊p�x�ݒ�
			ImGui::DragFloat("InitalVelocity", &hormingParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);		//�������x�ݒ�
			ImGui::DragFloat("Acceleration", &hormingParameter_.acceleration, 0.001f, 0.0f, 1.0f);			//�����x�ݒ�
			ImGui::DragFloat("FanRadius",&hormingParameter_.radius,0.01f,5.0f);								//��̔��a
			ImGui::SliderInt("NextShotCoolTime",&hormingParameter_.cooltime,0,120);							//���̒e��ł܂ł̃N�[���^�C��
			ImGui::SliderInt("AliveTime",&hormingParameter_.alivetime,1,300);								//�z�[�~���O��������
			ImGui::DragFloat("HormingAccuracy",&hormingParameter_.horming_accuracy,0.01f,0.0f,1.0f);		//�z�[�~���O���x
			ImGui::DragFloat("SetPositionSpeed",&hormingParameter_.setposition_speed,0.01f,1.0f);			//�|�W�V�����Ɍ������X�s�[�h
			ImGui::DragFloat("Length", &hormingParameter_.length, 1.0f, 0.0f, 100.0f);						//�L���[�u���΂������ݒ�
			ImGui::SliderFloat("CUBE SIZE", &hormingParameter_.cubeSize, 0.1f, 1.0f);						//�L���[�u�̃T�C�Y
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
			circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

			FinishState();
		}
	}
	if (change_hormingcube_num)
	{
		change_hormingcube_num = false;

		//�L���[�u�̐����ύX����Ă�����
		if (hormingParameter_.CUBE_NUM != hormingParameter_.OLD_CUBE_NUM)
		{
			//���݂̃L���[�u�̐���ݒ�
			hormingParameter_.OLD_CUBE_NUM = hormingParameter_.CUBE_NUM;

			delete[] hormingParameter_.cube;
			hormingParameter_.cube = new Cube[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.vec;
			hormingParameter_.vec = new Vector3[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.spawnvec;
			hormingParameter_.spawnvec = new ParameterVector[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.shot;
			hormingParameter_.shot = new bool[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.cooltimecount;
			hormingParameter_.cooltimecount = new int[hormingParameter_.CUBE_NUM];

			delete[] hormingParameter_.alivetimecount;
			hormingParameter_.alivetimecount = new int[hormingParameter_.CUBE_NUM];

			FinishState();
		}
	}
	//�|�W�V����������
	if (reset_position)
	{
		reset_position = false;
		SetPosition(init_pos);
	}
	//�p�����[�^������
	if (reset_parameter)
	{
		reset_parameter = false;
		//�������p�Ɋe�p�����[�^�쐬
		EnemyIdle::ENEMY_PARAMETER idleparameter;
		EnemyHigeCircleShot::ENEMY_PARAMETER circleshotparameter;
		EnemyHigeRush::ENEMY_PARAMETER rushparameter;
		EnemyHigeHorming::ENEMY_PARAMETER hormingparameter;

		//�e�p�����[�^�������Đݒ�
		SetIdleParameter(&idleparameter);
		SetRushParameter(&rushparameter);
		SetCircleShotParameter(&circleshotparameter);
		SetHormingParameter(&hormingparameter);

		InitParameter();
	}
}

void EnemyHige::InitParameter()
{
	//�g��U�����̃L���[�u�쐬
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];
	//�g��U���x�N�g���쐬
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

	//�z�[�~���O�̃L���[�u�쐬
	hormingParameter_.cube = new Cube[hormingParameter_.CUBE_NUM];
	//�z�[�~���O�x�N�g���쐬
	hormingParameter_.vec = new Vector3[hormingParameter_.CUBE_NUM];
	//�z�[�~���O�X�|�[���x�N�g���쐬
	hormingParameter_.spawnvec = new ParameterVector[hormingParameter_.CUBE_NUM];
	//�z�[�~���O�V���b�g�t���O�쐬
	hormingParameter_.shot = new bool[hormingParameter_.CUBE_NUM];
	//�z�[�~���O�J�E���^�쐬
	hormingParameter_.cooltimecount = new int[hormingParameter_.CUBE_NUM];
	//�z�[�~���O�������ԃJ�E���^�쐬
	hormingParameter_.alivetimecount = new int[hormingParameter_.CUBE_NUM];
}

void EnemyHige::InitParameterValue()
{
	//�g��U��������
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SetVisible(false);
		circleShotParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
	}

	//�z�[�~���O������
	for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].SetVisible(false);
		hormingParameter_.cube[i].SetPosition(0.0f,0.0f,0.0f);
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.shot[i] = false;
		hormingParameter_.cooltimecount[i] = 0;
		hormingParameter_.alivetimecount[i] = 0;
	}
}

void EnemyHige::DestParameter()
{
	if (circleShotParameter_.cube)
	{
		delete[] circleShotParameter_.cube;
	}
	if (circleShotParameter_.vec)
	{
		delete[] circleShotParameter_.vec;
	}
	if (hormingParameter_.cube)
	{
		delete[] hormingParameter_.cube;
	}
	if (hormingParameter_.vec)
	{
		delete[] hormingParameter_.vec;
	}
	if (hormingParameter_.spawnvec)
	{
		delete[] hormingParameter_.spawnvec;
	}
	if (hormingParameter_.shot)
	{
		delete[] hormingParameter_.shot;
	}
	if (hormingParameter_.cooltimecount)
	{
		delete[] hormingParameter_.cooltimecount;
	}
	if (hormingParameter_.alivetimecount)
	{
		delete[] hormingParameter_.alivetimecount;
	}
}
