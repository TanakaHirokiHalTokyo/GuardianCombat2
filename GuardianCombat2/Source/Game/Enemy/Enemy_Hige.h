#pragma once

#include "Enemy.h"

class StateEnemy;

class EnemyHige :public Enemy
{
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
private:
	StateEnemy* rotate_ = nullptr;				//回転制御
	StateEnemy* moveToPlayer_ = nullptr;		//プレイヤーに向かう制御
};