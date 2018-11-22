#pragma once

//===================================================================
//
//			指定した地点に移動させる
//
//===================================================================
#include "StateEnemy.h"
#include <d3dx9.h>

class StateEnemy_MoveToPoint :public StateEnemy
{
public:
	StateEnemy_MoveToPoint(D3DXVECTOR3 point ,float speed);	//コンストラクタ
	~StateEnemy_MoveToPoint();														//デストラクタ
	void Act(Enemy* enemy)override;													//実行
	void BeginDisplay(Enemy* enemy)override;									//事前表示
	void Display(Enemy* enemy)override;											//表示
	void EndDisplay(Enemy* enemy)override;										//終了表示

	bool GetArrial();
private:
	D3DXVECTOR3 point_ = D3DXVECTOR3(0,0,0);		//移動先
	bool askLength_ = false;												//移動先までの距離を求めたか。
	float length_ = 0.0f;														//移動先までの距離
	float speed_ = 0.0f;														//移動するスピード
	float transValue_ = 0.0f;												//移動した合計量
	bool arrival_ = false;													//移動先に到達したか
};