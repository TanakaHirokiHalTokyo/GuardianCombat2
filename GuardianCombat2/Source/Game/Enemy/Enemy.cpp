#include "Enemy.h"
#include "../XModel/XModel.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"	
#include "../../Sound/Sound.h"
#include "../../Effekseer/Effekseer.h"
#include <iostream>
#include <fstream>

Enemy::~Enemy()
{

}

void Enemy::JudgePause()
{
	if (isEdit_)
	{
		//F2 GamepadXボタンを押した際に敵は更新処理を行わなくなる。
		if (GetKeyboardTrigger(DIK_F2) || X_CONTROLLER::GetXcontrollerButtonTrigger(1, XINPUT_GAMEPAD_X))
		{
			enemypause_ = !enemypause_;
		}
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

void Enemy::SetPositionExploose(D3DXVECTOR3 pos)
{
	if (exploose_)
	{
		exploose_->SetPosition(pos);
	}
}

void Enemy::SetPositionExploose(float x, float y, float z)
{
	if (exploose_)
	{
		exploose_->SetPosition(x,y,z);
	}
}

void Enemy::PlayExploose()
{
	if (explooseSE_)
	{
		explooseSE_->PlaySoundA();
	}
	if (exploose_)
	{
		exploose_->Play();
	}
}

void Enemy::StopExploose()
{
	if (explooseSE_)
	{
		explooseSE_->StopSound();
	}
	if (exploose_)
	{
		exploose_->Stop();
	}
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
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
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

void Enemy::SaveBasisParameter(std::string filename)
{
	std::ofstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::out);
	file.write((const char*)&this->maxLife_,sizeof(this->maxLife_));
	file.write((const char*)&this->damageCutRate_,sizeof(this->damageCutRate_));
	file.close();
}

void Enemy::LoadBasisParameter(std::string filename)
{
	std::ifstream file;
	file.open("resource/" + filename + ".parameter", std::ios::binary | std::ios::in);
	if (file.fail())
	{
		MessageBoxA(NULL, "パラメータデータを読み込めませんでした。\nデフォルトデータを使用します。", "失敗", MB_OK | MB_ICONHAND);
	}
	else
	{
		file.read((char*)&this->maxLife_, sizeof(this->maxLife_));
		file.read((char*)&this->damageCutRate_, sizeof(this->damageCutRate_));
	}
	file.close();
}
