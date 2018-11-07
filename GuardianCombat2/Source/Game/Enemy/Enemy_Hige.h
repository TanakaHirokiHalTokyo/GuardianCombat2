#pragma once

#include "Enemy.h"

class StatePattern_Enemy;
class Sphere;
class DebugSphere;
class Cube;

class CircleVector
{
public:
	D3DXVECTOR3 vector;
};

namespace EnemyHigeRush							//突進状態のパラメータ
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.5f;						//スピード
		float length = 10.0f;					//突進距離
	};
}
namespace EnemyHigeCircleShot					//波状攻撃の情報
{
	struct ENEMY_PARAMETER
	{
		int CUBE_NUM = 10;						//波状攻撃時のキューブの数
		int OLD_CUBE_NUM = 10;					//前回のキューブの数
		float inital_velocity = 0.1f;			//キューブの飛んでいく初速度
		float acceleration = 0.001f;			//加速度
		float speed = inital_velocity;			//現在のスピード
		float length = 10.0f;					//キューブを飛ばす距離
		float cubeSize = 1.0f;					//キューブのサイズ
		Cube* cube = nullptr;					//キューブ情報
		CircleVector* vec = nullptr;			//ベクトル情報
	};
}


class EnemyHige :public Enemy
{
public:
	enum STATE
	{
		IDLE,					//待機状態
		RUSH,					//突進
		CIRCLESHOT,				//波状攻撃
		STATE_MAX,
	};
	const char* StateWord[STATE_MAX]
	{
		"IDLE",
		"RUSH",
		"CIRCLESHOT",
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

	//突進状態のパラメータ取得・設定
	EnemyHigeRush::ENEMY_PARAMETER GetRushParameter();
	void SetRushParameter(EnemyHigeRush::ENEMY_PARAMETER parameter);

	//波状攻撃のパラメータ取得・設定
	EnemyHigeCircleShot::ENEMY_PARAMETER GetCircleShotParameter();
	void SetCircleShotParameter(EnemyHigeCircleShot::ENEMY_PARAMETER parameter);

private:
	void DrawDebug();													//Debug表示
private:
	StatePattern_Enemy* statePattern_ = nullptr;						//ステート状態管理
	STATE state_ = IDLE;												//状態
	Sphere* collision_ = nullptr;										//コリジョン情報
	DebugSphere* debugCollision_ = nullptr;								//コリジョンを可視化するか

	EnemyHigeRush::ENEMY_PARAMETER rushParameter_ = {};					//突進状態のパラメータ情報
	EnemyHigeCircleShot::ENEMY_PARAMETER circleShotParameter_ = {};		//波状攻撃のパラメータ情報
};