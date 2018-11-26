#include "StateEnemyAvater_Attack.h"
#include "../Enemy_HigeAvater.h"
#include "../../Cube/Cube.h"
#include "../../../main.h"
#include "../../GameManager/GameManager.h"
#include "../../Player/Player.h"
#include "../../../Collision/Collision.h"

constexpr int COUNTER_ATTACK = 180;
constexpr int SHOOTING_COUNTER = 120;

void StateEnemyAvater_Attack::Action(EnemyHige_Avater * enemy)
{
	//“G‚ÌƒLƒ…[ƒuî•ñŽæ“¾
	if (!cube_){	cube_ = enemy->GetCube();	}

	if (!init_)
	{
		init_ = true;
		counter_ = Randrange_int(0,150);
	}
	counter_++;
	if (counter_ >= COUNTER_ATTACK)
	{
		shooting_ = true;
		counter_ = 0;
		shotVector_ = GameManager::GetPlayer()->GetPosition() - enemy->GetPosition();
		shotVector_.y = 0;
		D3DXVec3Normalize(&shotVector_, &shotVector_);
		cube_->SetVisible(true);
		cube_->SetPosition(enemy->GetPosition());
		cube_->GetCollision()->enable_ = true;
	}
	if (shooting_)
	{
		shootingcounter_++;
		if (shootingcounter_ >= SHOOTING_COUNTER)
		{
			shootingcounter_ = 0;
			shooting_ = false;
			cube_->SetVisible(false);
			cube_->GetCollision()->enable_ = false;
			cube_->SetHit(false);
		}

		cube_->SetPosition(cube_->GetPosition() + shotVector_ * 0.5f);
		cube_->SetPositionY(0.5f);
		cube_->SetRotation(cube_->GetRotate().x + 10.0f, 0.0f, cube_->GetRotate().z + 10.0f);
		cube_->GetCollision()->m_Pos = cube_->GetPosition();
	}
}

void StateEnemyAvater_Attack::BeginDisplay(EnemyHige_Avater * enemy)
{
}

void StateEnemyAvater_Attack::Display(EnemyHige_Avater * enemy)
{
}

void StateEnemyAvater_Attack::EndDisplay(EnemyHige_Avater * enemy)
{
}
