#pragma once

#include "Player.h"


class Camera;
class FPSPlayerState;
class Weapon;
class FPSPlayer_UI;

class FPSPlayer :public Player
{
public:
	const float FPSPLAYER_DEF_SPEED = 0.05f;		//Playerのデフォルトスピード

	FPSPlayer();					//コンストラクタ
	~FPSPlayer()override;			//デストラクタ
	void Init()override;			//初期化処理
	void Uninit()override;			//終了処理
	void Update()override;			//更新処理
	void BeginDraw()override;		//事前描画処理
	void Draw()override;			//描画処理
	void EndDraw()override;			//オブジェクトすべての描画が終わった後の処理

	Weapon* GetWeapon();

private:
	FPSPlayerState* movement_ = nullptr;	//プレイヤーの移動を制御
	Weapon* shotgun_ = nullptr;				//ショットガン情報
	FPSPlayer_UI* playerUI_ = nullptr;		//PlayerUI情報
};