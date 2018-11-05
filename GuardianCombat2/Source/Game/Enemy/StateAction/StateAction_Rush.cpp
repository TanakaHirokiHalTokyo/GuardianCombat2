#include "StateAction_Rush.h"
#include "../StateEnemy/StateEnemy_Rotate.h"
#include "../StateEnemy/StateEnemy_Rush.h"

StateAction_Rush::StateAction_Rush(Enemy* enemy)
{
	SetEnemy(enemy);		//“GÝ’è

	//‰ñ“]§Œäì¬
	rotate_ = new StateEnemy_Rotate();
	//“Ëi§Œäì¬
	rush_ = new StateEnemy_Rush();
}

StateAction_Rush::~StateAction_Rush()
{
	if (rotate_)
	{
		delete rotate_;
		rotate_ = nullptr;
	}
	if (rush_)
	{
		delete rush_;
		rush_ = nullptr;
	}
}

void StateAction_Rush::Action()
{
	//ƒvƒŒƒCƒ„[‚Ì‚Ù‚¤‚ð‚Ü‚¾U‚èŒü‚¢‚Ä‚¢‚È‚¢‚Æ‚«U‚èŒü‚«ˆ—‚ðs‚¤B
	if (!isRotate_)
	{
		//‰ñ“]ˆ—@ŽÀs
		rotate_->Act(enemy_);
		isRotate_ = true;;
	}
	else
	{
		EnemyHige* hige = (EnemyHige*)enemy_;
		//“Ëiˆ—@ŽÀs
		rush_->Act(hige);
	}
}

void StateAction_Rush::Display()
{
	EnemyHige* hige = (EnemyHige*)enemy_;
	//‰ñ“]ˆ—@•\Ž¦
	rotate_->Display(enemy_);
	rush_->Display(hige);
}

void StateAction_Rush::SetEnemy(Enemy * enemy)
{
	enemy_ = enemy;
}
