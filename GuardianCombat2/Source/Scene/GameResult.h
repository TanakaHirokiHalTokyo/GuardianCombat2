//============================================================================================
//
//		ゲーム結果シーン
//
//============================================================================================
#pragma once
#include "Scene.h"
#include <string>

class Texture;
class Sound;

class GameResult :public Scene
{
public:
	GameResult();
	~GameResult()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	void SaveSettings(std::string filename);
	void LoadSettings(std::string filename);
	
	int selectResultNumber_ = 0;				//選択中の結果番号 0: Win 1 : Lose
	Sound* bgm_ = nullptr;						//BGM
	Sound* selectSE_ = nullptr;				//選択効果音
	Texture* pressKey_ = nullptr;				//PressKeyテクスチャ
	Texture* resultWord_ = nullptr;			//リザルト結果文字
	Texture* resultBack_ = nullptr;			//リザルト背景
	float keyColor_[3] = { 0 };					//PressKey色
	float keyAlpha_ = 0.0f;						//PressKeyAlpha
	float wordWinColor_[3] = {0};				//勝利文字色
	float wordLoseColor_[3] = { 0 };			//敗北文字色
	float backWinColor_[3] = {0};				//勝利背景色
	float backLoseColor_[3] = { 0 };			//敗北背景色
	float scaleAddRate_ = 0.002f;				//スケール値拡大率
	float scaleRate_ = 1.0f;						//スケール値
	float alphaAddRate_ = 1.5f;				//α値加算値
};