#pragma once

#include "../Object.h"
#include "../../main.h"
#include <array>
#include <string>

class Vector3;
class XModel;
class Sound;
class CEffekseer;

//待機状態
namespace EnemyIdle
{
	struct ENEMY_PARAMETER
	{
		float speed = 0.01f;						//スピード
		float rot_angle = 0.1f;					//回転角度
		float approache_length = 1.0f;	//プレイヤーとの距離（これ以上は近づかない）
		std::array<int, 5> count = { 120, 100,80,60,40 };
		std::array<float, 4> hp_ratio_ = { 0.8f,0.6f,0.4f,0.2f };												//HPの割合
		std::array<double, 5> normalAttackLuck = { 0.9,0.8,0.7,0.65,0.6 };
		std::array<double, 5> specialAttackLuck = { 0.1 ,0.2,0.3,0.35,0.4};
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
	void DecreaseLife(float value) {
		life_ -= value;
		if (life_ < 0.0f) life_ = 0.0f;
	}
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

	void SetPositionExploose(D3DXVECTOR3 pos);		//爆発エフェクト座標設定
	void SetPositionExploose(float x,float y,float z);
	void PlayExploose();				//爆発音再生
	void StopExploose();				//爆発音停止

	EnemyIdle::ENEMY_PARAMETER GetIdleParameter();									//待機状態パラメータ取得
	void SetIdleParameter(EnemyIdle::ENEMY_PARAMETER* parameter);		//待機状態パラメータ設定

	inline float GetMoveSpeedToPoint() { return moveSpeedToPoint_; }				//設定した場所に向かうスピード
	inline ENEMY_TYPE GetEnemyType() { return enemyType_; }						//敵の種類取得
	inline size_t GetNormalAttackNum() { return normalAttackNum_; }				//通常攻撃の数取得
	inline size_t GetSpecialAttackNum() { return specialAttackNum_; }				//特殊攻撃の数取得
	inline void SetAutoAttack(bool flag) { autoAttack_ = flag; }							//オートアタック設定
	inline bool GetAutoAttack() { return autoAttack_; }										//オートアタックしているのか取得
	inline void SetInvincible(bool flag) { invincible_ = flag; }								//無敵状態設定
	inline bool GetInvincible() { return invincible_; }											//無敵状態か取得
	inline void SetEditMode(bool flag) { isEdit_ = flag; }										//EditMode設定
	inline bool GetEditMode()const { return isEdit_; }											//EditMode取得
	inline void SetCollisionEnable(bool flag) { isCollisionEnable_ = flag; }			//Collisionの当たり判定行うか設定
	inline bool GetCollisionEnable() { return isCollisionEnable_; }						//Collisionの当たり判定行うか取得
	inline void SetCollisionDPS(float value) { collisionDPS_ = value; }				//Collisionの当たり判定の際のDPS設定
	inline float GetCollisionDPS() { return collisionDPS_; }									//Collisionの当たり判定の際のDPS取得
	inline void SetDamageCutRate(float rate) { damageCutRate_ = rate; }			//ダメージカット率設定
	inline float GetDamageCutRate() { return damageCutRate_; }						//ダメージカット率取得
protected:
	void SaveIdleParameter(std::string filename);												//待機状態パラメータ保存
	void LoadIdleParameter(std::string filename);												//待機状態パラメータ読み込み
	void SaveBasisParameter(std::string filename);												//基本情報パラメータ保存
	void LoadBasisParameter(std::string filename);											//基本情報パラメータ読み込み

	CEffekseer* exploose_ = nullptr;																				//爆発エフェクト
	Sound* explooseSE_ = nullptr;																					//爆発音	（ヒット時）
	float maxLife_ = ENEMY_MAX_LIFE;																			//最大体力
	float damageCutRate_ = 0.0f;																						//ダメージカット率
	bool isEdit_ = false;																									//EditModeか
	bool isCollisionEnable_ = false;																					//Playerと体がぶつかったとき、判定を行うか
	float collisionDPS_ = 0.0f;																							//コリジョン判定を行う際のDPS
	ENEMY_TYPE enemyType_ = ENEMY_MAX;																//敵の種類
	float life_ = ENEMY_MAX_LIFE;																					//体力
	Vector3* vector_ = nullptr;																							//ベクトル情報
	XModel* model_ = nullptr;																							//モデル情報
	bool enemypause_ = false;																							//敵のポーズ状態
	EnemyIdle::ENEMY_PARAMETER idleParameter_;												//待機状態のパラメータ
	float moveSpeedToPoint_ = 0.1f;																				//移動地点に移動するスピード
	size_t normalAttackNum_ = 0;																				//通常攻撃の数
	size_t specialAttackNum_ = 0;																				//特殊攻撃の数
	bool autoAttack_ = false;																						//自動的に攻撃をしてくるか
	bool invincible_ = false;																							//無敵フラグ
};