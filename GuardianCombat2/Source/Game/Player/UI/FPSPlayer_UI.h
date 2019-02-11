#pragma once

//==========================================================
//
//		FPSPlayer ユーザーインターフェース
//
//==========================================================

class Texture;
class Player;

class FPSPlayer_UI
{
public:
	FPSPlayer_UI();
	~FPSPlayer_UI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	Player* player_ = nullptr;						//Playerポインタ
	Texture* lifeBar_ = nullptr;						//ライフバー
	Texture* lifeBarMax_ = nullptr;				//ライフバーの背景
	Texture* reticle_ = nullptr;						//レティクルテクスチャ
	Texture* damage_ = nullptr;					//DamageScreen
};