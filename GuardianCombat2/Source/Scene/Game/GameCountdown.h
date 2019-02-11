#pragma once
//====================================================================
//
//		ゲーム開始前のカウントダウンを制御
//
//====================================================================
class Texture;
class Sound;

class GameCountdown
{
public:
	GameCountdown();
	~GameCountdown();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	inline bool GetCountDown() { return isCountDown_; }			//カウントダウン中か取得
private:
	Sound* countSE_ = nullptr;			//カウント時の効果音
	Sound* startSE_ = nullptr;				//スタート時の効果音
	Texture* back_ = nullptr;				//背景テクスチャ
	Texture* count1_ = nullptr;				//カウント１テクスチャ
	Texture* count2_ = nullptr;				//カウント２テクスチャ
	Texture* count3_ = nullptr;				//カウント３テクスチャ
	float wordSize_ = 0.0f;					//文字のサイズ
	int index_ = 3;									//現在表示している数字
	bool isCountDown_ = true;				//カウントダウン中か
};