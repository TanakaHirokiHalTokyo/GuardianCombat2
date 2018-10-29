#pragma once

#include "Player.h"


class Camera;
class FPSPlayerState;

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

	Camera* GetCamera();

private:
	Camera* camera_ = nullptr;				//カメラ情報
	FPSPlayerState* movement_ = nullptr;	//プレイヤーの移動を制御
};