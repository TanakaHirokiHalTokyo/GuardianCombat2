#pragma once

//==========================================================
//
//		FPSPlayer ユーザーインターフェース
//
//==========================================================

class Texture;

class FPSPlayer_UI
{
public:
	const float RETICLE_SIZE = 50.0f;

	FPSPlayer_UI();
	~FPSPlayer_UI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	Texture* reticle_ = nullptr;		//レティクルテクスチャ
};