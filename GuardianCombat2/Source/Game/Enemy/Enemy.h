#pragma once

#include "../Object.h"
#include "../../main.h"
#include <array>

class Vector3;
class XModel;

//待機状態
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//スピード
		float rot_angle = 0.1f;					//回転角度
		std::array<int, 5> count = { 120, 100,80,60,40 };
		std::array<float, 4> hp_ratio_ = { 0.8f,0.6f,0.4f,0.2f };												//HPの割合
		std::array<double, 5> normalAttackLuck = { 0.9,0.9,0.9,0.9,0.9 };
		std::array<double, 5> specialAttackLuck = { 0.1 ,0.1,0.1,0.1,0.1};
		int idle__counter = 0;
	};
}

const float ENEMY_MAX_LIFE = 100.0f;

class Enemy :public Object
{
public:

	enum ENEMY_TYPE
	{
		ENEMY_HIGE,
		ENEMY_MAX
	};

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
	void SetVector(Vector3 vector);							//ベクトル設定
	void SetFront(D3DXVECTOR3 front);
	void SetUp(D3DXVECTOR3 up);
	void SetRight(D3DXVECTOR3 right);

	void SetDebugMode(bool flag) { debug_ = flag; }					//デバッグモード設定
	bool GetDebugMode() { return debug_; }						//デバッグモード取得

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);

	inline float GetMoveSpeedToPoint() { return moveSpeedToPoint_; }
	inline ENEMY_TYPE GetEnemyType() { return enemyType_; }
	inline size_t GetNormalAttackNum() { return normalAttackNum_; }
	inline size_t GetSpecialAttackNum() { return specialAttackNum_; }
	inline bool GetAutoAttack() { return autoAttack_; }
protected:
	ENEMY_TYPE enemyType_ = ENEMY_MAX;																//敵の種類
	float life_ = ENEMY_MAX_LIFE;																										//体力
	Vector3* vector_ = nullptr;																							//ベクトル情報
	XModel* model_ = nullptr;																							//モデル情報
	bool debug_ = false;																									//デバッグ状態にするか
	bool enemypause_ = false;																							//敵のポーズ状態
	EnemyIdle::ENEMY_PARAMETER idleParameter_ = {};												//待機状態のパラメータ
	float moveSpeedToPoint_ = 0.1f;																				//移動地点に移動するスピード
	size_t normalAttackNum_ = 0;																				//通常攻撃の数
	size_t specialAttackNum_ = 0;																				//特殊攻撃の数
	bool autoAttack_ = false;																						//自動的に攻撃をしてくるか
};