#pragma once

//=========================================================
//
//		ブラー
//
//=========================================================

class Texture;

class Blur
{
public:
	Blur();													//コンストラクタ
	~Blur();												//デストラクタ
	void BeginDraw();								//通常の描画処理の前に挟む
	void EndDraw();									//通常の描画処理が終わった際にする処理

	void SetUseBlur(bool flag);					//ブラーを使うかのセット
	bool GetUseBlur();								//ブラーを使っているかの取得
	void SetAlpha(int value);						//ブラーテクスチャのぼかし具合設定
	void SetStrength(float value);				//ブラーの強さ設定
private:
	bool useBlur_ = false;							//現在ブラーを使っているのか
	Texture* blurTex_ = nullptr;				//ブラーテクスチャ
	int alpha_ = 150;									//ブラーテクスチャα
	float strength_ = 1.0f;							//ブラーの強さ
};