#include "WeaponRelative.h"
#include "../Weapon.h"
#include "../../Player.h"

void WeaponRelative::Act(Weapon * weapon)
{
	//武器の親になっているプレイヤーを取得
	const auto& player = weapon->GetPlayer();
}
