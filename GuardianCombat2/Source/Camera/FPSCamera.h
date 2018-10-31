#pragma once

#include "Camera.h"
#include "../Game/Player/Player.h"

class DeviceConfig;

class FPSCamera :public Camera
{
public:
	FPSCamera();							//コンストラクタ
	~FPSCamera();							//デストラクタ
	void Init()override;					//初期化
	void Uninit()override;					//終了処理
	void Update()override;					//更新
	void BeginDraw()override;				//事前描画
	void Draw()override;					//描画
	void EndDraw()override;					//すべてのオブジェクトが描画し終わった際の処理

	void SetPlayer(Player* player);

private:
	void CameraAtUpdate();							//中視点更新

	const float INIT_HEIGHT = 0.5f;					//初期ｙ座標
	const float CAMERA_AT_LENGTH = 100.0f;			//中視点の距離
	Player* player_ = nullptr;						//Player情報
	D3DXVECTOR3 MouseRotate = D3DXVECTOR3(0,0,0);	//マウス制御回転角
	DeviceConfig* deviceConfig_;					//デバイス情報
	float sensitivity_ = 2.0f;						//コントローラー感度
	float mouseSensi_ = 0.1f;						//マウス感度
};