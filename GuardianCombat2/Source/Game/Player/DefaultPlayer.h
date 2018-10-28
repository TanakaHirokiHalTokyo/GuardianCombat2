#pragma once
#include "Player.h"

class SceneSkinMesh;			//スキンメッシュ
class AnimationPattern;			//アニメーションの遷移クラス
class PlayerCamera;				
class Camera;					
class StatePlayer;				

class DefaultPlayer :public Player
{
public:
	enum ANIMATION_TYPES
	{
		RUN,
		JUMPING_KICK,
		JUMPING_PUNCH,
		PUNCH_COMBO,
		PUNCH_RIGHT,
		PUNCH_LEFT,
		JUMP,
		KICK_COMBO,
		KICK_LEFT,
		KICK_RIGHT,
		WALK_BACK,
		WALK_FORWARD,
		IDLE,
		ANIMATION_MAX,
	};
public:
	DefaultPlayer();
	~DefaultPlayer();
	void Init()override;	
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	SceneSkinMesh* GetAnimModel();
	void ChangeState(StatePlayer* state);

	void RotatePlayer();

private:
	StatePlayer* playerState_ = nullptr;
	SceneSkinMesh* animationModel_ = nullptr;
	AnimationPattern* animationPattern_ = nullptr;
	PlayerCamera* playerCamera_ = nullptr;
	Camera* usingCamera_ = nullptr;
};