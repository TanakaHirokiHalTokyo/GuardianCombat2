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
