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
#include "../Player/Player.h"
#include "../Effect/Effect.h"
#include "../../Effekseer/Effekseer.h"
#include "../../Game/MeshField/MeshField.h"
#include "../../DirectXRenderer.h"	
#include "../../Texture/Texture.h"
#include "Enemy_HigeAvater.h"

const D3DXVECTOR3 init_pos = D3DXVECTOR3(-0.1f,0.0f,-8.0f);
const float EnemyHige::BURSTX = 3.5f;
const float EnemyHige::BURSTY = 3.5f;
const float EnemyHige::BURSTZ = 8.5f;

EnemyHige::EnemyHige()
{
	enemyType_ = ENEMY_HIGE;

	normalAttackNum_ = TELEPORTETION;
	specialAttackNum_ = STATE_MAX - (normalAttackNum_ + 1);

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
	enemyCollisions_.emplace_back(collision_);

	ring_ = Object::Create<XModel>();
	ring_->SetModelType(XModel::MODEL_RING);
	ring_->SetScale(0.5f, 0.5f, 0.5f);

	//�p�����[�^������
	InitParameter();

	this->LoadIdleParameter("EnemyHige_Idle");
	this->LoadCircleShotParameter("EnemyHige_CircleShot");
	this->LoadRushParameter("EnemyHige_Rush");
	this->LoadHormingParameter("EnemyHige_Horming");
	this->LoadTeleportParameter("EnemyHige_Teleport");
	this->LoadSummonsParameter("EnemyHige_Summons");
	this->LoadBurstParameter("EnemyHige_BurstShot");
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
	DestParameter();
}

void EnemyHige::Init()
{
	//������
	life_ = 100.0f;

	SetScale(0.3f, 0.3f, 0.3f);
	SetPosition(init_pos);
	SetRotation(0,0,0);
	SetSpeed(0.02f);

	//���f����񏉊���
	model_->SetScale(GetScale());
	model_->SetPosition(GetPosition());
	model_->SetRotation(GetRotate());

	//�x�N�g��������
	Player* player = GameManager::GetPlayer();
	D3DXVECTOR3 front = player->GetPosition() - GetPosition();
	D3DXVec3Normalize(&front,&front);
	vector_->SetFront(front);
	vector_->SetUp(0, 1.0f, 0);
	D3DXVECTOR3 right;
	//�E�x�N�g��
	D3DXVec3Cross(&right, &vector_->GetUp(), &vector_->GetFront());
	D3DXVec3Normalize(&right, &right);
	vector_->SetRight(right);
	
	//Collision������
	collision_->rad = 0.5f;

	FinishState();

	InitCircleParameterValue();
	InitHormingParameterValue();
	InitSummonsParameterValue();
	InitBurstShotParameterValue();
	InitTeleportParameterValue();
}

void EnemyHige::Uninit()
{
}

void EnemyHige::Update()
{
	if (life_ <= 0.0f)
	{
		GameManager::GameOver(true);
		return;
	}

	//�G�����̃|�[�Y���[�h�K�p����̂����f
	JudgePause();

	if (!enemypause_)
	{
		//�X�e�[�g������s
		statePattern_->Act();

		//�͈͓��Ɏ��߂�
		float lengthX = fabsf(GetPosition().x);
		float lengthZ = fabsf(GetPosition().z);

		if (lengthX > FIELD_SIZE)
		{
			if (GetPosition().x > 0) { SetPositionX(FIELD_SIZE); }
			else { SetPositionX(-FIELD_SIZE); }
		}
		if (lengthZ > FIELD_SIZE)
		{
			if (GetPosition().z > 0) { SetPositionZ(FIELD_SIZE); }
			else { SetPositionZ(-FIELD_SIZE); }
		}

		//���f�����X�V
		model_->SetRotation(GetRotate());
		model_->SetPosition(GetPosition());
		model_->SetScale(GetScale());

		ring_->SetPosition(GetPosition());
		ring_->SetPositionY(ring_->GetPosition().y + ring_->GetScale().x);
		collision_->pos = GetPosition();
		collision_->pos.y = collision_->pos.y + collision_->rad;

		for (int i = 0; i < summonsParameter_.summons_num; i++)
		{
			summonsParameter_.avater[i].Update();
		}
	}
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

	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].BeginDraw();
	}

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
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].Draw();
	}


	if (this->GetEditMode())
	{
		//�f�o�b�O�\���@�X�e�[�g�E�p�����[�^�\��
		DrawDebug();
	}
	statePattern_->Display();
}

void EnemyHige::EndDraw()
{
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].EndDraw();
	}

	statePattern_->EndDisplay();
}

EnemyHige::STATE EnemyHige::GetState()
{
	return state_;		//��Ԏ擾
}

void EnemyHige::SetState(STATE state)
{
	idleParameter_.idle__counter = 0;
	state_ = state;		//��Ԑݒ�
	statePattern_->ChangeState();
}
void EnemyHige::FinishState()
{
	enemyBurstCollision_->enable_ = false;
	SetPositionY(MeshField::GetHeight(GetPosition()));
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
//==================================================
//		�ːi�p�����[�^�ۑ�
//		filename : �����o���t�@�C����
//==================================================
void EnemyHige::SaveRushParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->rushParameter_.length, sizeof(float));
	file.write((const char*)&this->rushParameter_.speed, sizeof(float));
	file.write((const char*)&this->rushParameter_.attack, sizeof(float));
	file.close();
	
}
//============================================================
//		�ːi�U���p�����[�^�Ǎ�
//		filename : �ǂݍ��݃t�@�C�����iresource/ + .parameter�ȗ��j
//=============================================================
void EnemyHige::LoadRushParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B","���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->rushParameter_.length, sizeof(float));
		file.read((char*)&this->rushParameter_.speed, sizeof(float));
		file.read((char*)&this->rushParameter_.attack, sizeof(float));
	}
	file.close();
}
EnemyHigeCircleShot::ENEMY_PARAMETER EnemyHige::GetCircleShotParameter()
{
	return circleShotParameter_;		//�g��U���擾
}
void EnemyHige::SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER* parameter)
{
	circleShotParameter_ = *parameter;	//�g��U���ݒ�
}
//=====================================================================
//		�g��U���p�����[�^�ۑ�
//		filename : �����o���t�@�C����
//======================================================================
void EnemyHige::SaveCircleShotParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->circleShotParameter_.acceleration,sizeof(circleShotParameter_.acceleration));
	file.write((const char*)&this->circleShotParameter_.cubeSize,sizeof(circleShotParameter_.cubeSize));
	file.write((const char*)&this->circleShotParameter_.CUBE_NUM,sizeof(circleShotParameter_.CUBE_NUM));
	file.write((const char*)&this->circleShotParameter_.inital_velocity,sizeof(circleShotParameter_.inital_velocity));
	file.write((const char*)&this->circleShotParameter_.length,sizeof(circleShotParameter_.length));
	file.close();

	for (int i = 0; i < this->circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].SaveParameter(filename + "_cube");
		circleShotParameter_.effect[i].SaveParameter(filename + "_effect");
	}
	
}
//========================================================================
//		�g��U���p�����[�^�ǂݍ���
//		filename : �ǂݍ��݃t�@�C����
//========================================================================
void EnemyHige::LoadCircleShotParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->circleShotParameter_.acceleration, sizeof(circleShotParameter_.acceleration));
		file.read((char*)&this->circleShotParameter_.cubeSize, sizeof(circleShotParameter_.cubeSize));
		file.read((char*)&this->circleShotParameter_.CUBE_NUM, sizeof(circleShotParameter_.CUBE_NUM));
		file.read((char*)&this->circleShotParameter_.inital_velocity, sizeof(circleShotParameter_.inital_velocity));
		file.read((char*)&this->circleShotParameter_.length, sizeof(circleShotParameter_.length));
	}
	file.close();

	this->ReCreateCircleParameter();
	
	for (int i = 0; i < this->circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.cube[i].LoadParameter(filename + "_cube");
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
		circleShotParameter_.effect[i].LoadParameter(filename + "_effect");
	}
}
EnemyHigeHorming::ENEMY_PARAMETER EnemyHige::GetHormingParameter()
{
	return hormingParameter_;			//�z�[�~���O�p�����[�^�擾
}
void EnemyHige::SetHormingParameter(EnemyHigeHorming::ENEMY_PARAMETER * parameter)
{
	hormingParameter_ = *parameter;		//�z�[�~���O�p�����[�^�ݒ�
}
//========================================================================
//		Horming�p�����[�^�ۑ�
//		filename : �������݃t�@�C����
//========================================================================
void EnemyHige::SaveHormingParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->hormingParameter_.acceleration,sizeof(hormingParameter_.acceleration));
	file.write((const char*)&this->hormingParameter_.alivetime,sizeof(hormingParameter_.alivetime));
	file.write((const char*)&this->hormingParameter_.cooltime,sizeof(hormingParameter_.cooltime));
	file.write((const char*)&this->hormingParameter_.cubeSize,sizeof(hormingParameter_.cubeSize));
	file.write((const char*)&this->hormingParameter_.CUBE_NUM,sizeof(hormingParameter_.CUBE_NUM));
	file.write((const char*)&this->hormingParameter_.fanangle, sizeof(hormingParameter_.fanangle));
	file.write((const char*)&this->hormingParameter_.horming_accuracy,sizeof(hormingParameter_.horming_accuracy));
	file.write((const char*)&this->hormingParameter_.inital_velocity,sizeof(hormingParameter_.inital_velocity));
	file.write((const char*)&this->hormingParameter_.radius,sizeof(hormingParameter_.radius));
	file.write((const char*)&this->hormingParameter_.setposition_speed,sizeof(hormingParameter_.setposition_speed));
	file.close();

	for (int i = 0; i < this->hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].SaveParameter(filename + "_cube");
		hormingParameter_.effect[i].SaveParameter(filename + "_effect");
	}
}
//================================================================
//			�z�[�~���O�p�����[�^�ǂݍ���
//			filename : �ǂݍ��݃t�@�C����
//================================================================
void EnemyHige::LoadHormingParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->hormingParameter_.acceleration, sizeof(hormingParameter_.acceleration));
		file.read((char*)&this->hormingParameter_.alivetime, sizeof(hormingParameter_.alivetime));
		file.read((char*)&this->hormingParameter_.cooltime, sizeof(hormingParameter_.cooltime));
		file.read((char*)&this->hormingParameter_.cubeSize, sizeof(hormingParameter_.cubeSize));
		file.read((char*)&this->hormingParameter_.CUBE_NUM, sizeof(hormingParameter_.CUBE_NUM));
		file.read((char*)&this->hormingParameter_.fanangle, sizeof(hormingParameter_.fanangle));
		file.read((char*)&this->hormingParameter_.horming_accuracy, sizeof(hormingParameter_.horming_accuracy));
		file.read((char*)&this->hormingParameter_.inital_velocity, sizeof(hormingParameter_.inital_velocity));
		file.read((char*)&this->hormingParameter_.radius, sizeof(hormingParameter_.radius));
		file.read((char*)&this->hormingParameter_.setposition_speed, sizeof(hormingParameter_.setposition_speed));
	}
	file.close();

	this->ReCreateHormingParameter();
	for (int i = 0; i < this->hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.cube[i].LoadParameter(filename + "_cube");
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.effect[i].LoadParameter(filename + "_effect");
	}
}
EnemyHigeTeleportation::ENEMY_PARAMETER EnemyHige::GetTeleportParameter()
{
	return teleportationParameter_;
}
void EnemyHige::SetTeleportParameter(EnemyHigeTeleportation::ENEMY_PARAMETER * parameter)
{
	teleportationParameter_ = *parameter;
}
//=====================================================================
//		�e���|�[�g�p�����[�^�ۑ�
//		filename : �������݃t�@�C����
//=====================================================================
void EnemyHige::SaveTeleportParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->teleportationParameter_.distance,sizeof(teleportationParameter_.distance));
	file.close();
}
//====================================================================
//		�e���|�[�g�p�����[�^�Ǎ�
//		filename : �ǂݍ��݃t�@�C����	s5e201938
//=====================================================================
void EnemyHige::LoadTeleportParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->teleportationParameter_.distance, sizeof(teleportationParameter_.distance));
	}
	file.close();
}
EnemyHigeSummons::ENEMY_PARAMETER EnemyHige::GetSummonsParameter()
{
	return summonsParameter_;
}
void EnemyHige::SetSummonsParameter(EnemyHigeSummons::ENEMY_PARAMETER * parameter)
{
	summonsParameter_ = *parameter;
}
//==========================================================================
//			���g�����p�����[�^�ۑ�
//			filename : �������݃t�@�C����
//==========================================================================
void EnemyHige::SaveSummonsParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->summonsParameter_.attack_interval,sizeof(summonsParameter_.attack_interval));
	file.write((const char*)&this->summonsParameter_.cube_size,sizeof(summonsParameter_.cube_size));
	file.write((const char*)&this->summonsParameter_.cube_speed,sizeof(summonsParameter_.cube_speed));
	file.write((const char*)&this->summonsParameter_.summons_max,sizeof(summonsParameter_.summons_max));
	file.write((const char*)&this->summonsParameter_.summons_num,sizeof(summonsParameter_.summons_num));
	file.write((const char*)&this->summonsParameter_.trans_length,sizeof(summonsParameter_.trans_length));
	file.write((const char*)&this->summonsParameter_.trans_speed,sizeof(summonsParameter_.trans_speed));
	file.write((const char*)&this->moveSpeedToPoint_,sizeof(moveSpeedToPoint_));
	file.close();

	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.avater[i].GetCube()->SaveParameter(filename + "_cube");
	}
}
//==========================================================================
//			���g�����p�����[�^�Ǎ�
//			filename : �ǂݍ��݃t�@�C����
//==========================================================================
void EnemyHige::LoadSummonsParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->summonsParameter_.attack_interval, sizeof(summonsParameter_.attack_interval));
		file.read((char*)&this->summonsParameter_.cube_size, sizeof(summonsParameter_.cube_size));
		file.read((char*)&this->summonsParameter_.cube_speed, sizeof(summonsParameter_.cube_speed));
		file.read((char*)&this->summonsParameter_.summons_max, sizeof(summonsParameter_.summons_max));
		file.read((char*)&this->summonsParameter_.summons_num, sizeof(summonsParameter_.summons_num));
		file.read((char*)&this->summonsParameter_.trans_length, sizeof(summonsParameter_.trans_length));
		file.read((char*)&this->summonsParameter_.trans_speed, sizeof(summonsParameter_.trans_speed));
		file.read((char*)&this->moveSpeedToPoint_, sizeof(moveSpeedToPoint_));
	}

	file.close();

	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].GetCube()->LoadParameter(filename + "_cube");
	}
}
EnemyHigeBurstShot::ENEMY_PARAMETER EnemyHige::GetBurstParameter()
{
	return burstParameter_;
}
void EnemyHige::SetBurstParameter(EnemyHigeBurstShot::ENEMY_PARAMETER* parameter)
{
	burstParameter_ = *parameter;
}
//==============================================================================
//			�o�[�X�g�p�����[�^�ۑ�
//			filename : �������݃t�@�C����
//==============================================================================
void EnemyHige::SaveBurstParameter(const std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->burstParameter_.dps,sizeof(burstParameter_.dps));
	file.write((const char*)&this->burstParameter_.effect_size, sizeof(burstParameter_.effect_size));
	file.write((const char*)&this->burstParameter_.blur_color[0], sizeof(burstParameter_.blur_color[0]));
	file.write((const char*)&this->burstParameter_.blur_color[1], sizeof(burstParameter_.blur_color[1]));
	file.write((const char*)&this->burstParameter_.blur_color[2], sizeof(burstParameter_.blur_color[2]));
	file.write((const char*)&this->burstParameter_.blur_alpha, sizeof(burstParameter_.blur_alpha));
	file.write((const char*)&this->burstParameter_.isblur, sizeof(burstParameter_.isblur));
	file.close();
}
//==============================================================================
//			�o�[�X�g�p�����[�^�Ǎ�
//			filename : �ǂݍ��݃t�@�C����
//==============================================================================
void EnemyHige::LoadBurstParameter(const std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->burstParameter_.dps, sizeof(burstParameter_.dps));
		file.read((char*)&this->burstParameter_.effect_size, sizeof(burstParameter_.effect_size));
		file.read((char*)&this->burstParameter_.blur_color[0], sizeof(burstParameter_.blur_color[0]));
		file.read((char*)&this->burstParameter_.blur_color[1], sizeof(burstParameter_.blur_color[1]));
		file.read((char*)&this->burstParameter_.blur_color[2], sizeof(burstParameter_.blur_color[2]));
		file.read((char*)&this->burstParameter_.blur_alpha, sizeof(burstParameter_.blur_alpha));
		file.read((char*)&this->burstParameter_.isblur, sizeof(burstParameter_.isblur));

		burstParameter_.effect->SetScale(burstParameter_.effect_size, burstParameter_.effect_size, burstParameter_.effect_size);
	}
	file.close();
}
void EnemyHige::DrawDebug()
{
	static int listbox_item_current = 1;			//���X�g�{�b�N�X�̏����I��ԍ�
	static bool changeState = false;				//��Ԃ�ύX������
	static bool reset_position = false;			//�G�̃|�W�V�������Z�b�g�t���O
	static bool reset_parameter = false;		//�p�����[�^���Z�b�g�t���O

	ImGui::SetNextWindowPos(ImVec2(0.0f,0.0f));
	ImGui::SetNextWindowSize(ImVec2((float)ScreenWidth/ 3.0f,(float)ScreenHeight));
	//Imgui �\��
	ImGui::Begin(u8"�G�̃p�����[�^���");
	ImGui::Text(u8"F2�L�[ : �G�|�[�Y");
	ImGui::Text(u8"���W %f %f %f",GetPosition().x, GetPosition().y, GetPosition().z);
	ImGui::Text(u8"�O�����x�N�g��\t %f %f %f",vector_->GetFront().x, vector_->GetFront().y, vector_->GetFront().z);
	ImGui::Text(u8"�E�����x�N�g��\t %f %f %f", vector_->GetRight().x, vector_->GetRight().y, vector_->GetRight().z);
	ImGui::Text(u8"������x�N�g��\t %f %f %f", vector_->GetUp().x, vector_->GetUp().y, vector_->GetUp().z);
	ImGui::Text(u8"�G�̕��g�̐����Ă��鐔 : %d",summonsParameter_.avater_alive);
	reset_position = ImGui::Button(u8"�G�̍��W������");
	reset_parameter = ImGui::Button(u8"�p�����[�^��񏉊���");
	if (ImGui::Button(u8"�p�����[�^�ۑ�"))
	{
		this->SaveIdleParameter("EnemyHige_Idle");
		this->SaveRushParameter("EnemyHige_Rush");
		this->SaveCircleShotParameter("EnemyHige_CircleShot");
		this->SaveHormingParameter("EnemyHige_Horming");
		this->SaveTeleportParameter("EnemyHige_Teleport");
		this->SaveSummonsParameter("EnemyHige_Summons");
		this->SaveBurstParameter("EnemyHige_BurstShot");
	}
	ImGui::Checkbox(u8"�I�[�g�A�^�b�N",&autoAttack_);
	ImGui::Checkbox(u8"���G", &invincible_);											//���G
	ImGui::SliderFloat(u8"�̗�",&life_,0.0f,ENEMY_MAX_LIFE);
	ImGui::Text(u8"�U���p�^�[�� : %s",StateWord[state_]);

	//���ɐݒ肷��STATE
	changeState = ImGui::ListBox(u8"�U���p�^�[���w��i�N���b�N�j", &listbox_item_current, StateWord, IM_ARRAYSIZE(StateWord), 4);
	if (ImGui::CollapsingHeader(u8"��ԕω�HP�̊����p�����[�^"))
	{
		//�G�̏�ԕω�������HP�̊����ݒ�
		std::array<float, 4> value;

		value = idleParameter_.hp_ratio_;
		if (ImGui::DragFloat(u8"1�i�K�� : HP����", &value[0],0.01f,0.0f,1.0f))
		{
			if (value[0] > idleParameter_.hp_ratio_[1] && value[0] <= 1.0f) { idleParameter_.hp_ratio_[0] = value[0]; }
		}
		if (ImGui::DragFloat(u8"2�i�K�� : HP����", &value[1], 0.01f, 0.01f, 1.0f))
		{
			if (value[1] > idleParameter_.hp_ratio_[2] && value[1] < idleParameter_.hp_ratio_[0]) { idleParameter_.hp_ratio_[1] = value[1]; }
		}
		if (ImGui::DragFloat(u8"3�i�K�� : HP����", &value[2], 0.01f, 0.01f, 1.0f))
		{
			if (value[2] > idleParameter_.hp_ratio_[3] && value[2] < idleParameter_.hp_ratio_[1]) { idleParameter_.hp_ratio_[2] = value[2]; }
		}
		if (ImGui::DragFloat(u8"4�i�K�� : HP����", &value[3], 0.01f, 0.01f, 1.0f))
		{
			if (value[3] < idleParameter_.hp_ratio_[2] && value[3] >= 0.0f) { idleParameter_.hp_ratio_[3] = value[3]; }
		}
	}
	if (ImGui::CollapsingHeader(u8"�ʏ�U���m���p�����[�^"))
	{
		std::array<double, 5> normal = idleParameter_.normalAttackLuck;
		if (ImGui::InputDouble(u8"1�i�K�� : �ʏ�U���m��", &normal[0]))
		{
			if (normal[0] >= 0.0 && normal[0] <= 1.0) { idleParameter_.normalAttackLuck[0] = normal[0]; }
		}
		if (ImGui::InputDouble(u8"2�i�K�� : �ʏ�U���m��", &normal[1]))
		{
			if (normal[1] >= 0.0 && normal[1] <= 1.0) { idleParameter_.normalAttackLuck[1] = normal[1]; }
		}
		if (ImGui::InputDouble(u8"3�i�K�� : �ʏ�U���m��", &normal[2]))
		{
			if (normal[2] >= 0.0 && normal[2] <= 1.0) { idleParameter_.normalAttackLuck[2] = normal[2]; }
		}
		if (ImGui::InputDouble(u8"4�i�K�� : �ʏ�U���m��", &normal[3]))
		{
			if (normal[3] >= 0.0 && normal[3] <= 1.0) { idleParameter_.normalAttackLuck[3] = normal[3]; }
		}
		if (ImGui::InputDouble(u8"5�i�K�� : �ʏ�U���m��", &normal[4]))
		{
			if (normal[4] >= 0.0 && normal[4] <= 1.0) { idleParameter_.normalAttackLuck[4] = normal[4]; }
		}
		for (size_t i = 0; i < idleParameter_.specialAttackLuck.size(); i++)
		{
			idleParameter_.specialAttackLuck[i] = 1.0 - idleParameter_.normalAttackLuck[i];
		}
	}
	
	if (ImGui::CollapsingHeader(u8"�ҋ@��ԏڍ�"))
	{
		//�ҋ@��Ԃ̃p�����[�^�ݒ�
		if (ImGui::TreeNode(u8"�ҋ@��ԃp�����[�^"))
		{
			ImGui::DragFloat(u8"�v���C���[�ɂ��̋����܂ŋ߂Â�", &idleParameter_.approache_length, 0.1f, 0.0f, 30.0f);
			ImGui::DragFloat(u8"�X�s�[�h", &idleParameter_.speed, 0.001f, 0.0f, 0.1f);			//�X�s�[�h�ݒ�
			ImGui::DragFloat(u8"�U��������x", &idleParameter_.rot_angle, 0.01f, 0.01f, 6.0f);		//��]�X�s�[�h�ݒ�
			std::array<int, 5> count;
			count = idleParameter_.count;
			if (ImGui::DragInt(u8"1�i�K�� : �ҋ@����", &count[0],1,0,300))
			{
				if (count[0] >= 0) { idleParameter_.count[0] = count[0]; }
			}
			if (ImGui::DragInt(u8"2�i�K�� : �ҋ@����", &count[1], 1, 0, 300))
			{
				if (count[1] >= 0) { idleParameter_.count[1] = count[1]; }
			}
			if (ImGui::DragInt(u8"3�i�K�� : �ҋ@����", &count[2], 1, 0, 300))
			{
				if (count[2] >= 0) { idleParameter_.count[2] = count[2]; }
			}
			if (ImGui::DragInt(u8"4�i�K�� : �ҋ@����", &count[3], 1, 0, 300))
			{
				if (count[3] >= 0) { idleParameter_.count[3] = count[3]; }
			}
			if (ImGui::DragInt(u8"5�i�K�� : �ҋ@����", &count[4], 1, 0, 300))
			{
				if (count[4] >= 0) { idleParameter_.count[4] = count[4]; }
			}
			
			ImGui::TreePop();
		}
	}
	//�ʏ�U���p�����[�^�ݒ�
	if (ImGui::CollapsingHeader(u8"�ʏ�U���ڍ�"))
	{
		//==========================================================================================================
		//		Rush
		//=========================================================================================================
		//�ːi��Ԃ̃p�����[�^�ݒ�
		if (ImGui::TreeNode(u8"�ːi�U��(Rush)�p�����[�^"))
		{
			ImGui::DragFloat(u8"�X�s�[�h", &rushParameter_.speed, 0.01f, 0.0f, 1.0f);			//�X�s�[�h�ݒ�
			ImGui::DragFloat(u8"�ːi���鋗��", &rushParameter_.length, 1.0f, 0.0f, 100.0f);			//�ːi�����ݒ�
			ImGui::DragFloat(u8"1�b�ԓ�����̃_���[�W��",&rushParameter_.attack,0.1f,0.0f,30.0f);
			ImGui::TreePop();
		}
		//===========================================================================================================
		//		CircleShot
		//=======================================================================================================
		{
			static bool change_cube_num = false;			//�L���[�u�̐���ύX������
			static bool change_effect_limit = true;			//�G�t�F�N�g�̐������Ԃ�ύX������
			static int effect_limit = 90;								//�G�t�F�N�g�̐�������
			static bool change_effect_color = false;			//�G�t�F�N�g�̐F��ύX������
			static float effect_color[3];								//�G�t�F�N�g�̐F
			static float effect_size = 1.0f;							//�G�t�F�N�g�̃T�C�Y
			static bool change_effect_size = false;			//�G�t�F�N�g�̃T�C�Y��ύX������

			static bool init = false;
			if (!init)
			{
				init = true;
				effect_color[0] = circleShotParameter_.effect[0].GetColor().r;
				effect_color[1] = circleShotParameter_.effect[0].GetColor().g;
				effect_color[2] = circleShotParameter_.effect[0].GetColor().b;

				effect_limit = circleShotParameter_.effect[0].GetLimit();
				effect_size = circleShotParameter_.effect[0].GetScale();
			}
			

			//�g��U���̃p�����[�^�ݒ�
			if (ImGui::TreeNode(u8"�g��U��(CIRCLESHOT)�p�����[�^"))
			{
				change_cube_num = ImGui::SliderInt(u8"�e�̐�", &circleShotParameter_.CUBE_NUM, 1, 20);				//�L���[�u���ݒ�
				ImGui::DragFloat(u8"�����x", &circleShotParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);						//�������x�ݒ�
				ImGui::DragFloat(u8"�����x", &circleShotParameter_.acceleration, 0.001f, 0.0f, 1.0f);							//�����x�ݒ�
				ImGui::DragFloat(u8"��΂�����", &circleShotParameter_.length, 1.0f, 0.0f, 100.0f);												//�L���[�u���΂������ݒ�
				ImGui::SliderFloat(u8"�e�̑傫��", &circleShotParameter_.cubeSize, 0.1f, 1.0f);											//�L���[�u�̃T�C�Y
				change_effect_color = ImGui::ColorEdit3(u8"�G�t�F�N�g�F",effect_color);															//�G�t�F�N�g�̃J���[�ݒ�
				change_effect_size = ImGui::DragFloat(u8"�G�t�F�N�g�T�C�Y",&effect_size,0.1f,0.0f,10.0f);											//�G�t�F�N�g�T�C�Y�̐ݒ�
				change_effect_limit = ImGui::SliderInt(u8"�G�t�F�N�g��������",&effect_limit,0,150);													//�G�t�F�N�g�̐������Ԑݒ�

				ImGui::TreePop();
			}
			//�L���[�u�̐���ύX�����Ƃ�
			if (change_cube_num)
			{
				change_cube_num = false;

				//�L���[�u�̐����ύX����Ă�����
				if (circleShotParameter_.CUBE_NUM != circleShotParameter_.OLD_CUBE_NUM)
				{
					change_effect_color = true;
					change_effect_limit = true;
					change_effect_size = true;

					//���݂̃L���[�u�̐���ݒ�
					circleShotParameter_.OLD_CUBE_NUM = circleShotParameter_.CUBE_NUM;

					//�g��U���p�����[�^�č쐬
					ReCreateCircleParameter();

					InitCircleParameterValue();
					FinishState();
				}
			}
			//�G�t�F�N�g�̐F���ύX����Ă�����
			if (change_effect_color)
			{
				change_effect_color = false;
				for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
				{
					circleShotParameter_.effect[i].SetColor(effect_color[0], effect_color[1], effect_color[2]);
				}
			}
			//�G�t�F�N�g�̐������ԕύX����Ă�����
			if (change_effect_limit)
			{
				change_effect_limit = false;
				for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
				{
					circleShotParameter_.effect[i].SetLimit(effect_limit);
				}
			}
			//�G�t�F�N�g�̃T�C�Y�ύX����Ă�����
			if (change_effect_size)
			{
				change_effect_size = false;
				for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
				{
					circleShotParameter_.effect[i].SetScale(effect_size);
				}
			}
		}
		//====================================================================================================
		//		Horming
		//====================================================================================================
		{
			static bool change_hormingcube_num = false;																//�z�[�~���O�L���[�u�̐���ύX������
			static bool change_horming_effect_color = false;															//�z�[�~���O�̃G�t�F�N�g�J���[��ύX�������B
			static float horming_effect_color[3];																				//�G�t�F�N�g�J���[�z��
			static float effect_size = 1.0f;																							//�G�t�F�N�g�T�C�Y
			static int limit_time = hormingParameter_.effect[0].GetLimit();										//�G�t�F�N�g��������
			static bool change_limit = false;																						//�G�t�F�N�g�̐������ԕύX������
			static bool change_effect_size = false;																			//�G�t�F�N�g�̃T�C�Y�ύX������
			static bool change_accel = false;																					//�����x�ύX������
			static float accel = hormingParameter_.acceleration;													//�����x

			static bool init = false;
			if (!init)
			{
				init = true;
				horming_effect_color[0] = hormingParameter_.effect[0].GetColor().r;
				horming_effect_color[1] = hormingParameter_.effect[0].GetColor().g;
				horming_effect_color[2] = hormingParameter_.effect[0].GetColor().b;

				effect_size = hormingParameter_.effect[0].GetScale();
			}

			//�z�[�~���O�̃p�����[�^�ݒ�
			if (ImGui::TreeNode(u8"�z�[�~���O(HORMING)�p�����[�^"))
			{
				change_hormingcube_num = ImGui::SliderInt(u8"�e�̐�", &hormingParameter_.CUBE_NUM, 1, 20);						//�L���[�u���ݒ�
				ImGui::SliderFloat(u8"�W�J�p�x", &hormingParameter_.fanangle, 60.0f, 180.0f, "%.1f", 1.0f);										//��̊p�x�ݒ�
				ImGui::DragFloat(u8"�����x", &hormingParameter_.inital_velocity, 0.01f, 0.0f, 10.0f);													//�������x�ݒ�
				change_accel = ImGui::DragFloat(u8"�����x", &accel, 0.001f, 0.0f, 1.0f);																		//�����x�ݒ�
				ImGui::DragFloat(u8"�e�̍��W�㏸�l", &hormingParameter_.radius, 0.1f, 1.0f, 10.0f);													//��̔��a
				ImGui::SliderInt(u8"���̒e��ł܂ł̎���", &hormingParameter_.cooltime, 0, 120);												//���̒e��ł܂ł̃N�[���^�C��
				ImGui::SliderInt(u8"�e�̐�������", &hormingParameter_.alivetime, 1, 300);																//�z�[�~���O��������
				ImGui::DragFloat(u8"�z�[�~���O���x", &hormingParameter_.horming_accuracy, 0.01f, 0.0f, 1.0f);								//�z�[�~���O���x
				ImGui::DragFloat(u8"�e�̍��W�㏸���x", &hormingParameter_.setposition_speed, 0.01f, 1.0f);									//�|�W�V�����Ɍ������X�s�[�h
				ImGui::SliderFloat(u8"�e�̑��x", &hormingParameter_.cubeSize, 0.1f, 1.0f);																//�L���[�u�̃T�C�Y
				change_horming_effect_color = ImGui::ColorEdit3(u8"�G�t�F�N�g�F", horming_effect_color);										//�G�t�F�N�g�̃J���[�ݒ�
				change_limit = ImGui::SliderInt(u8"�G�t�F�N�g��������", &limit_time, 70, 250);															//�G�t�F�N�g�̐������Ԑݒ�
				change_effect_size = ImGui::DragFloat(u8"�G�t�F�N�g�T�C�Y", &effect_size, 0.1f, 0.0f, 20.0f, "%.2f");						//�G�t�F�N�g�̃T�C�Y��ݒ�
				ImGui::TreePop();
			}

			if (change_hormingcube_num)
			{
				change_hormingcube_num = false;

				//�L���[�u�̐����ύX����Ă�����
				if (hormingParameter_.CUBE_NUM != hormingParameter_.OLD_CUBE_NUM)
				{
					change_horming_effect_color = true;
					change_limit = true;

					//���݂̃L���[�u�̐���ݒ�
					hormingParameter_.OLD_CUBE_NUM = hormingParameter_.CUBE_NUM;

					//�z�[�~���O�p�����[�^�č쐬
					ReCreateHormingParameter();

					InitHormingParameterValue();
					FinishState();
				}
			}
			//Horming Effect Color Change
			if (change_horming_effect_color)
			{
				change_horming_effect_color = false;

				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetColor(
						horming_effect_color[0],
						horming_effect_color[1],
						horming_effect_color[2]);
				}
			}
			//�������ԕύX����Ă�����X�V
			if (change_limit)
			{
				change_limit = false;
				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetLimit(limit_time);
				}
			}
			//�G�t�F�N�g�̃T�C�Y�X�V����Ă�����X�V
			if(change_effect_size)
			{
				change_effect_size = false;
				for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
				{
					hormingParameter_.effect[i].SetScale(effect_size);
				}
			}
			//�����x�ύX����Ă�����
			if (change_accel)
			{
				change_accel = false;
				hormingParameter_.acceleration = accel;
			}

		}
		//====================================================================================================
		//		Teleportation
		//====================================================================================================
		{
			if (ImGui::TreeNode(u8"�e���|�[�g(Teleport)�p�����[�^"))
			{
				ImGui::DragFloat(u8"�e���|�[�g��̃v���C���[�Ƃ̋���",&teleportationParameter_.distance,0.1f,0.0f,20.0f);		//�v���C���[�Ƃ̋�����ݒ�
				ImGui::TreePop();
			}
		}
	}
	if (ImGui::CollapsingHeader(u8"����U���ڍ�"))
	{
		//=====================================================================================================
		//			SummonsAvater
		//=====================================================================================================
		{
			static bool change_summon_num = false;

			if (ImGui::TreeNode(u8"���g����(SUMMONAVATER)�p�����[�^"))
			{
				if (ImGui::Button(u8"���ׂĂ̕��g����"))
				{
					for (int i = 0; i < summonsParameter_.summons_max; i++)
					{
						summonsParameter_.avater_alive = 0;
						summonsParameter_.avater[i].Init();
						summonsParameter_.avater[i].SetVisible(false);
						summonsParameter_.avater[i].GetCube()->GetCollision()->enable_ = false;
					}
				}
				change_summon_num = ImGui::SliderInt(u8"�������镪�g�̐�", &summonsParameter_.summons_num, 0, summonsParameter_.summons_max);
				ImGui::DragFloat(u8"���g���ړ����鋗��", &summonsParameter_.trans_length, 0.01f, 0.0f, 7.0f);
				ImGui::DragFloat(u8"���g���ړ�����X�s�[�h", &moveSpeedToPoint_, 0.01f, 0.01f, summonsParameter_.trans_length);
				ImGui::SliderFloat(u8"�e�̃T�C�Y",&summonsParameter_.cube_size,0.0f,3.0f);
				ImGui::SliderFloat(u8"�e�̃X�s�[�h", &summonsParameter_.cube_speed, 0.01f, 1.0f);
				ImGui::SliderInt(u8"���̍U��������܂ł̎���",&summonsParameter_.attack_interval,10,240);
				ImGui::TreePop();
			}
			if (change_summon_num)
			{
				change_summon_num = false;
				if (summonsParameter_.summons_num != summonsParameter_.avater_old_num)
				{
					summonsParameter_.avater_old_num = summonsParameter_.summons_num;
					ReCreateSummonsParameter();
					InitSummonsParameterValue();
					FinishState();
				}
			}
		}
		//=====================================================================================================
		//			BurstShot
		//=====================================================================================================
		{
			if (ImGui::TreeNode(u8"�o�[�X�g(BURSTSHOT)�p�����[�^"))
			{
				ImGui::Checkbox(u8"�u���[���g�p���邩",&burstParameter_.isblur);
				ImGui::DragFloat(u8"�o�[�X�g�̑傫��", &burstParameter_.effect_size, 0.01f, 0.1f, 3.0f);
				ImGui::DragFloat(u8"1�b�Ԃɗ^����_���[�W", &burstParameter_.dps, 0.1f, 0.0f, 100.0f);
				ImGui::ColorEdit3(u8"�u���[�e�N�X�`���̐F",burstParameter_.blur_color);
				ImGui::DragInt(u8"�u���[�e�N�X�`���̃A���t�@�l",&burstParameter_.blur_alpha,1);
				ImGui::TreePop();
			}
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
		EnemyHigeSummons::ENEMY_PARAMETER summonsparameter;

		//�p�����[�^���
		DestParameter();

		//�e�p�����[�^�������Đݒ�
		SetIdleParameter(&idleparameter);
		SetRushParameter(&rushparameter);
		SetCircleShotParameter(&circleshotparameter);
		SetHormingParameter(&hormingparameter);
		SetSummonsParameter(&summonsparameter);

		InitParameter();								//�p�����[�^��񏉊���
		InitCircleParameterValue();			//�g��U���̏�񏉊���
		InitHormingParameterValue();		//�z�[�~���O�̏�񏉊���
		InitSummonsParameterValue();
	}
}

void EnemyHige::InitParameter()
{
	for (size_t i = 0; i < idleParameter_.specialAttackLuck.size(); i++)
	{
		idleParameter_.specialAttackLuck[i] = 1.0 - idleParameter_.normalAttackLuck[i];
	}

	//==========================================================
	//			�g��U���@������
	//==========================================================
	//�g��U�����̃L���[�u�쐬
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];
	//�g��U���x�N�g���쐬
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];
	//�g��U���̃G�t�F�N�g�쐬
	circleShotParameter_.effect = new AdditionEffect[circleShotParameter_.CUBE_NUM];

	InitCircleParameterValue();

	//==========================================================
	//			�z�[�~���O������
	//==========================================================
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
	//�z�[�~���O�X�s�[�h�ϐ��쐬
	hormingParameter_.speed = new float[hormingParameter_.CUBE_NUM];	
	//�G�t�F�N�g�쐬
	hormingParameter_.effect = new AdditionEffect[hormingParameter_.CUBE_NUM];

	InitHormingParameterValue();

	//=========================================================
	//			�e���|�[�g������
	//=========================================================
	teleportationParameter_.effect = new CEffekseer(CEffekseer::Effect_Teleport);
	InitTeleportParameterValue();

	//=========================================================
	//			���g�����̏�����
	//=========================================================
	summonsParameter_.avater = new EnemyHige_Avater[summonsParameter_.summons_max];
	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].SetParent(this);
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.trans_length = 3.0f;
		summonsParameter_.trans_speed = 0.1f;

	}
	InitSummonsParameterValue();
	//=========================================================
	//			�o�[�X�g�V���b�g������
	//=========================================================
	burstParameter_.effect = new CEffekseer(CEffekseer::Effect_Burst);
	burstParameter_.collision = new OBB();
	enemyBurstCollision_ = burstParameter_.collision;
	InitBurstShotParameterValue();
}
void EnemyHige::DestParameter()
{
	if (circleShotParameter_.cube)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.cube);
	}
	if (circleShotParameter_.vec)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.vec);
	}
	if (circleShotParameter_.effect)
	{
		SAFE_DELETE_ARRAY(circleShotParameter_.effect);
	}
	if (hormingParameter_.cube)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.cube);
	}
	if (hormingParameter_.vec)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.vec);
	}
	if (hormingParameter_.spawnvec)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.spawnvec);
	}
	if (hormingParameter_.shot)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.shot);
	}
	if (hormingParameter_.cooltimecount)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.cooltimecount);
	}
	if (hormingParameter_.alivetimecount)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.alivetimecount);
	}
	if (hormingParameter_.speed)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.speed);
	}
	if (hormingParameter_.effect)
	{
		SAFE_DELETE_ARRAY(hormingParameter_.effect);
	}
	if (teleportationParameter_.effect)
	{
		SAFE_DELETE(teleportationParameter_.effect);
	}
	if (summonsParameter_.avater)
	{
		SAFE_DELETE_ARRAY(summonsParameter_.avater);
	}
	if (burstParameter_.effect)
	{
		burstParameter_.effect->Uninit();
		SAFE_DELETE(burstParameter_.effect);
	}
}

void EnemyHige::ReCreateCircleParameter()
{
	delete[] circleShotParameter_.cube;
	circleShotParameter_.cube = new Cube[circleShotParameter_.CUBE_NUM];

	delete[] circleShotParameter_.vec;
	circleShotParameter_.vec = new ParameterVector[circleShotParameter_.CUBE_NUM];

	delete[] circleShotParameter_.effect;
	circleShotParameter_.effect = new AdditionEffect[circleShotParameter_.CUBE_NUM];
}

void EnemyHige::ReCreateHormingParameter()
{
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

	delete[] hormingParameter_.speed;
	hormingParameter_.speed = new float[hormingParameter_.CUBE_NUM];

	delete[] hormingParameter_.effect;
	hormingParameter_.effect = new AdditionEffect[hormingParameter_.CUBE_NUM];
}

void EnemyHige::ReCreateSummonsParameter()
{
	for (int i = 0; i < summonsParameter_.summons_max; i++)
	{
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
	}
}

void EnemyHige::InitCircleParameterValue()
{
	//�g��U��������
	for (int i = 0; i < circleShotParameter_.CUBE_NUM; i++)
	{
		circleShotParameter_.effect[i].SetVisible(false);
		circleShotParameter_.cube[i].Init();
		circleShotParameter_.cube[i].SetVisible(false);
		circleShotParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		circleShotParameter_.cube[i].SetScale(circleShotParameter_.cubeSize, circleShotParameter_.cubeSize, circleShotParameter_.cubeSize);
	}
}

void EnemyHige::InitHormingParameterValue()
{
	//�z�[�~���O������
	for (int i = 0; i < hormingParameter_.CUBE_NUM; i++)
	{
		hormingParameter_.effect[i].SetVisible(false);
		hormingParameter_.cube[i].Init();
		hormingParameter_.cube[i].SetVisible(false);
		hormingParameter_.cube[i].SetPosition(0.0f, 0.0f, 0.0f);
		hormingParameter_.cube[i].SetScale(hormingParameter_.cubeSize, hormingParameter_.cubeSize, hormingParameter_.cubeSize);
		hormingParameter_.shot[i] = false;
		hormingParameter_.cooltimecount[i] = 0;
		hormingParameter_.alivetimecount[i] = 0;
		hormingParameter_.speed[i] = hormingParameter_.inital_velocity;
		hormingParameter_.spawnvec[i].vector = D3DXVECTOR3(0,1.0f,0);
		hormingParameter_.vec[i] = *vector_;
	}
}
void EnemyHige::InitTeleportParameterValue()
{
	teleportationParameter_.distance = 1.0f;
	teleportationParameter_.effect->Init();
	teleportationParameter_.effect->RepeatEffect(false);
	teleportationParameter_.effect->SetVisible(false);
	teleportationParameter_.effect->SetPosition(GetPosition());
	teleportationParameter_.effect->SetScale(1.0f, 1.0f, 1.0f);
}
void EnemyHige::InitSummonsParameterValue()
{
	for (int i = 0; i < summonsParameter_.summons_num; i++)
	{
		summonsParameter_.avater[i].Init();
		summonsParameter_.avater[i].SetVisible(false);
		summonsParameter_.trans_length = 3.0f;
		summonsParameter_.trans_speed = 0.1f;
	}
}
void EnemyHige::InitBurstShotParameterValue()
{
	burstParameter_.effect->Init();
	burstParameter_.effect->SetVisible(false);
	burstParameter_.collision->enable_ = false;
	burstParameter_.collision->m_fLength[0] = BURSTX;
	burstParameter_.collision->m_fLength[1] = BURSTY;
	burstParameter_.collision->m_fLength[2] = BURSTZ;
	burstParameter_.collision->m_NormaDirect[0] = D3DXVECTOR3(1.0f,0.0f,0.0f);
	burstParameter_.collision->m_NormaDirect[1] = D3DXVECTOR3(0.0f,1.0f,0.0f);
	burstParameter_.collision->m_NormaDirect[2] = D3DXVECTOR3(0.0f,0.0f,1.0f);
	burstParameter_.collision->object_ = this;
	burstParameter_.collision->m_Pos = GetPosition();
}