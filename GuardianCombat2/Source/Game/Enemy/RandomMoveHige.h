//==========================================================
//
//			指定した範囲内をランダムに動く敵
//
//==========================================================
#pragma once

#include "../Object.h"

class XModel;
class Vector3;

class RandomMoveHige :public Object
{
public:
	struct Range
	{
		float min;
		float max;
	};

	RandomMoveHige();
	~RandomMoveHige()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void Rotate();

	//		範囲設定
	inline void SetRangeX(float min, float max) { rangeX_.min = min; rangeX_.max = max; }
	inline void SetRangeY(float min, float max) { rangeY_.min = min; rangeY_.max = max; }
	inline void SetRangeZ(float min, float max) { rangeZ_.min = min; rangeZ_.max = max; }
private:
	D3DXMATRIX world_;																//ワールド行列
	XModel* model_ = nullptr;														//モデル
	Vector3* enemy_vector_ = nullptr;											//敵のベクトル
	Range rangeX_= { 0.0f };															//Xの範囲
	Range rangeY_= { 0.0f };															//Yの範囲
	Range rangeZ_= { 0.0f };															//Zの範囲
	float length_ = 0.0f;																	//次の地点までの距離
	D3DXVECTOR3 next_position_ = D3DXVECTOR3(0,0,0);		//次の地点
	int timer_ = 0;																			//目的地点付近までずっといけなかった場合、時間経過で次の地点算出
};
