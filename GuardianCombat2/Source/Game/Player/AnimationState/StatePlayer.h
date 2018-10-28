#pragma once

constexpr float ANIMATION_SHIFTTIME = 0.5f;
constexpr float ANIMATION_SPEED_IDLE = 1.0f;
constexpr float ANIMATION_SPEED_WALK = 1.5f;
constexpr float ANIMATION_SPEED_RUN = 1.5f;
constexpr float ANIMATION_SPEED_KICK = 2.0f;

#define WALK_FRONT_KEY	(DIK_W)
#define WALK_BACK_KEY	(DIK_S)
#define SPRINT_KEY		(DIK_LSHIFT)
#define KICK_KEY		(DIK_J)
#define PUNCH_KEY		(DIK_K)
#define JUMP_KEY		(DIK_SPACE)

class DefaultPlayer;
class StatePlayer
{
public:
	virtual void UpdateKeyboard(DefaultPlayer* player) = 0;
};