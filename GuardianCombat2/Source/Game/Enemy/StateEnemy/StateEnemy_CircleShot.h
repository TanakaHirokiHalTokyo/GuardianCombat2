#pragma once

//============================================================
//
//		波状攻撃
//
//============================================================

#include "StateEnemyHige.h"
#include <d3d9.h>
#include <d3dx9.h>

class Cube;

class StateEnemy_CircleShot :public StateEnemyHige
{
public:
	void Act(EnemyHige* enemy)override;				//実行
	void BeginDisplay(EnemyHige* enemy)override;	//事前表示
	void Display(EnemyHige* enemy)override;			//表示
	void EndDisplay(EnemyHige* enemy)override;		//表示終了

private:
	bool spawn_ = false;									//スポーンさせたか
	D3DXVECTOR3 *vec_ = nullptr;					//ベクトル情報
	float length = 0.0f;										//進んだ距離
};