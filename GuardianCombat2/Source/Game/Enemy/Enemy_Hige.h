#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;

namespace EnemyHigeIdle							//待機状態のパラメータ
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.1f;						//スピード
		float rot_angle = 0.1f;					//回転角度
	};
}
namespace EnemyHigeRush							//突進状態のパラメータ
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//スピード
		float length = 10.0f;					//突進距離
	};
}


class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,					//待機状態
		RUSH,					//突進
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
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

	//行動を終えた際にIDLE状態に
	void FinishState();

	//Idle状態のパラメータ取得・設定
	EnemyHigeIdle::ENEMY_PARAMETER GetIdleParameter();
	void SetIdleParameter(EnemyHigeIdle::ENEMY_PARAMETER parameter);

	//突進状態のパラメータ取得・設定
	EnemyHigeRush::ENEMY_PARAMETER GetRushParameter();
	void SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER parameter);

private:
	void DrawDebug();										//Debug表示
private:
	StatePattern_Enemy* statePattern_ = nullptr;			//ステート状態管理
	STATE state_ = IDLE;									//状態
	Sphere* collision_ = nullptr;							//コリジョン情報
	DebugSphere* debugCollision_ = nullptr;					//コリジョンを可視化するか

	EnemyHigeIdle::ENEMY_PARAMETER idleParameter_ = {};		//待機状態のパラメータ情報
	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};		//突進状態のパラメータ情報
};