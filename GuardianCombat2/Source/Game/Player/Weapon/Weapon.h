#pragma once

//========================================================
//
//		Weaponクラス
//
//========================================================

#include "../../Object.h"	

class Player;
class XModel;

class Weapon:public Object
{
public:
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	void SetPlayer(Player* player) { player_ = player; }
	Player* GetPlayer() { return player_; }

protected:
	void SetAttack(float Attack) { attack_ = Attack; }
	float GetAttack() { return attack_; }
	
protected:
	float attack_ = 1.0f;				//攻撃力
	Player* player_ = nullptr;			//プレイヤー情報。アタッチする際などに使用
	XModel* model_ = nullptr;			//銃のモデル
};