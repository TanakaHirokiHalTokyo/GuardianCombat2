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

void Player::SetCamera(Camera * camera)
{
	camera_ = camera;
}

Camera * Player::GetCamera()
{
	return camera_;
}

float Player::GetLife()
{
	return life_;
}

void Player::DecreaseLife(float value)
{
	life_ -= value;
	if (life_ < 0.0f)
	{
		life_ = 0.0f;
	}
}
