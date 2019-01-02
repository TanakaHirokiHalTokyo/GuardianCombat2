#pragma once

//========================================================
//
//		Weaponクラス
//
//========================================================

#include "../../Object.h"	

class Player;
class XModel;
class WeaponState;
class Bullet;
class Vector3;

class Weapon:public Object
{
public:
	virtual ~Weapon() {};
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	//親となるプレイヤーを設定・取得
	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

	//相対座標取得
	D3DXVECTOR3 GetRelativePosition() { return relativePosition_; }

	//弾の取得
	Bullet* GetBullet() { return bullet_; }

protected:
	//攻撃力設定・取得
	void SetAttack(float Attack) { attack_ = Attack; }
	float GetAttack() { return attack_; }
	
	//相対座標設定
	void SetRelativePosition(D3DXVECTOR3 pos) { relativePosition_ = pos; }
	void SetRelativePosition(float x, float y, float z) { relativePosition_ = D3DXVECTOR3(x, y, z); }

	//弾の設定
	void SetBullet(Bullet* bullet) { bullet_ = bullet; }
	
protected:
	void ShotBullet(int BulletNum);

protected:
	float attack_ = 1.0f;																						//攻撃力
	Player* player_ = nullptr;																			//プレイヤー情報。アタッチする際などに使用
	XModel* model_ = nullptr;																			//銃のモデル
	WeaponState* relative_ = nullptr;																//相対関係制御
	D3DXVECTOR3 relativePosition_ = D3DXVECTOR3(0,0,0);						//相対座標
	Bullet* bullet_ = nullptr;																				//弾の情報
	float fireRate_ = 10.0f;																				//1秒間に何発うてるのか。
	size_t rateCount_ = 0;																					//連射時カウンタ
};