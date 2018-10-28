#include "Player.h"
#include "../../Vector3/Vector3.h"

Vector3* Player::GetVector()
{
	return vector_;
}

void Player::SetOnLand(bool flag)
{
	onLand_ = flag;
}

bool Player::GetOnLand()
{
	return onLand_;
}
