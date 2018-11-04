#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;

class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,
		STATE_MAX,
	};
	const std::string StateWord[STATE_MAX]
	{
		"IDLE",
	};
public:
	EnemyHige();
	~EnemyHige();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	const float DEFAULT_SPEED = 0.1f;			//デフォルトスピード

	STATE GetState();
	void SetState(STATE state);

	

private:
	void DrawDebug();								//Debug表示
private:
	StatePattern_Enemy* statePattern_ = nullptr;	//ステート状態管理
	STATE state_ = IDLE;							//状態
	Sphere* collision_ = nullptr;					//コリジョン情報
	DebugSphere* debug;
};