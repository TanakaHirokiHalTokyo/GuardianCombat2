#include "Enemy.h"
#include "../XModel/XModel.h"
#include "../../Vector3/Vector3.h"
#include "../../DInput/DirectInput.h"
#include "../../XInput/xcontroller.h"	

Enemy::~Enemy()
{

}

void Enemy::JudgePause()
{
	//F2 GamepadXボタンを押した際に敵は更新処理を行わなくなる。
	if (GetKeyboardTrigger(DIK_F2) || X_CONTROLLER::GetXcontrollerButtonTrigger(0,XINPUT_GAMEPAD_X))
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
