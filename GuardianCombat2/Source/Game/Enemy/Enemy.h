#pragma once

#include "../Object.h"

class Vector3;
class XModel;

//待機状態
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//スピード
		float rot_angle = 0.1f;						//回転角度
	};
}

class Enemy :public Object
{
public:
	virtual ~Enemy();
	virtual void Init() {}
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	void SetLife(float value) { life_ = value; }						//体力設定
	float GetLife() { return life_; }									//体力取得
	void SetSpeed(float value) { idleParameter_.speed = value; }		//スピード設定
	float GetSpeed() { return idleParameter_.speed; }				//スピード取得
	void JudgePause();

	XModel* GetModel();											//モデル取得
	Vector3* GetVector();											//ベクトル取得

	void SetDebugMode(bool flag) { debug_ = flag; }					//デバッグモード設定
	bool GetDebugMode() { return debug_; }						//デバッグモード取得

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);

protected:
	float life_ = 100.0f;										//体力
	Vector3* vector_ = nullptr;								//ベクトル情報
	XModel* model_ = nullptr;								//モデル情報
	bool debug_ = false;										//デバッグ状態にするか
	bool enemypause_ = false;								//敵のポーズ状態
	EnemyIdle::ENEMY_PARAMETER idleParameter_ = {};	//待機状態のパラメータ

};