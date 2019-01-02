#include "Enemy.h"
#include "../XModel/XModel.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"	
#include <iostream>
#include <fstream>

Enemy::~Enemy()
{

}

void Enemy::JudgePause()
{
	//F2 GamepadX�{�^�����������ۂɓG�͍X�V�������s��Ȃ��Ȃ�B
	if (GetKeyboardTrigger(DIK_F2))
	{
		enemypause_ = !enemypause_;
	}
}

XModel * Enemy::GetModel()
{
	return model_;
}

Vector3 * Enemy::GetVector()
{
	return vector_;
}

void Enemy::SetVector(Vector3  vector)
{
	vector_ = &vector;
}

void Enemy::SetFront(D3DXVECTOR3 front)
{
	vector_->SetFront(front);
}

void Enemy::SetUp(D3DXVECTOR3 up)
{
	vector_->SetUp(up);
}

void Enemy::SetRight(D3DXVECTOR3 right)
{
	vector_->SetRight(right);
}

EnemyIdle::ENEMY_PARAMETER Enemy::GetIdleParameter()
{
	return idleParameter_;
}

void Enemy::SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter)
{
	idleParameter_ = *parameter;
}

void Enemy::SaveIdleParameter(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->idleParameter_.count, sizeof(idleParameter_.count));
	file.write((const char*)&this->idleParameter_.hp_ratio_, sizeof(idleParameter_.hp_ratio_));
	file.write((const char*)&this->idleParameter_.normalAttackLuck, sizeof(idleParameter_.normalAttackLuck));
	file.write((const char*)&this->idleParameter_.rot_angle,sizeof(idleParameter_.rot_angle));
	file.write((const char*)&this->idleParameter_.specialAttackLuck,sizeof(idleParameter_.specialAttackLuck));
	file.write((const char*)&this->idleParameter_.speed,sizeof(idleParameter_.speed));
	file.write((const char*)&this->idleParameter_.approache_length, sizeof(idleParameter_.approache_length));
	file.close();
}

void Enemy::LoadIdleParameter(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "�p�����[�^�f�[�^��ǂݍ��߂܂���ł����B\n�f�t�H���g�f�[�^���g�p���܂��B", "���s", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->idleParameter_.count, sizeof(idleParameter_.count));
		file.read((char*)&this->idleParameter_.hp_ratio_, sizeof(idleParameter_.hp_ratio_));
		file.read((char*)&this->idleParameter_.normalAttackLuck, sizeof(idleParameter_.normalAttackLuck));
		file.read((char*)&this->idleParameter_.rot_angle, sizeof(idleParameter_.rot_angle));
		file.read((char*)&this->idleParameter_.specialAttackLuck, sizeof(idleParameter_.specialAttackLuck));
		file.read((char*)&this->idleParameter_.speed, sizeof(idleParameter_.speed));
		file.read((char*)&this->idleParameter_.approache_length, sizeof(idleParameter_.approache_length));
	}
	file.close();
}
