#pragma once
//============================================================
//
//		テレポート
//
//============================================================
#include "StateEnemyHige.h"
#include <d3dx9.h>

class Player;

class StateEnemy_Teleportation :public StateEnemyHige
{
public:
	const float DIVE_SPEED = 0.01f;

	void Act(EnemyHige* enemy)override;
	void BeginDisplay(EnemyHige* enemy)override;
	void Display(EnemyHige* enemy)override;
	void EndDisplay(EnemyHige* enemy)override;
private:
	bool setVector_ = false;																		//ベクトルを設定したか
	bool playEffect_ = false;																		//エフェクトを再生したか
	float diveValue_ = 1.0f;																		//潜る量
	float divingValue_ = 0.0f;																	//潜っている量
	D3DXVECTOR3 teleportVector_ = D3DXVECTOR3(0,0,0);				//プレイヤーの位置からみてテレポートの向き
	Player* player_ = nullptr;																	//Player情報
};