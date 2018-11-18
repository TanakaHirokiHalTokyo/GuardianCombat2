#include "StateAction_Teleportation.h"
#include "../StateEnemy/StateEnemy_Teleportation.h"

StateAction_Teleportation::StateAction_Teleportation(Enemy* enemy)
{
	SetEnemy(enemy);		//“G‚ðÝ’è
	teleportation_ = new StateEnemy_Teleportation();
}

StateAction_Teleportation::~StateAction_Teleportation()
{
	delete teleportation_;
	teleportation_ = nullptr;
}

void StateAction_Teleportation::Action()
{
	teleportation_->Act((EnemyHige*)enemy_);
}

void StateAction_Teleportation::BeginDisplay()
{
	teleportation_->BeginDisplay((EnemyHige*)enemy_);
}

void StateAction_Teleportation::Display()
{
	teleportation_->Display((EnemyHige*)enemy_);
}

void StateAction_Teleportation::EndDisplay()
{
	teleportation_->EndDisplay((EnemyHige*)enemy_);
}
