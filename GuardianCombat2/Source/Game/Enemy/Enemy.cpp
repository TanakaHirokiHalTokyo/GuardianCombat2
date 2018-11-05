#include "Enemy.h"
#include "../XModel/XModel.h"
#include "../../Vector3/Vector3.h"

Enemy::~Enemy()
{

}

XModel * Enemy::GetModel()
{
	return model_;
}

Vector3 * Enemy::GetVector()
{
	return vector_;
}

EnemyIdle::ENEMY_PARAMETER Enemy::GetIdleParameter()
{
	return idleParameter_;
}

void Enemy::SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter)
{
	idleParameter_ = *parameter;
}
