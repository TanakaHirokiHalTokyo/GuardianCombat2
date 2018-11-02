#pragma once

#include "../Object.h"

class Vector3;
class XModel;

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

	void SetLife(float value) { life_ = value; }	//体力設定
	float GetLife() { return life_; }				//体力取得
	void SetSpeed(float value) { speed_ = value; }	//スピード設定
	float GetSpeed() { return speed_; }				//スピード取得

	XModel* GetModel();								//モデル取得
	Vector3* GetVector();							//ベクトル取得

	void SetDebugMode(bool flag) { debug_ = flag; }
	bool GetDebugMode() { return debug_; }
protected:
	float life_ = 100.0f;							//体力
	float speed_ = 0.01f;							//スピード
	Vector3* vector_ = nullptr;						//ベクトル情報
	XModel* model_ = nullptr;						//モデル情報
	bool debug_ = false;							//デバッグ状態にするか
};