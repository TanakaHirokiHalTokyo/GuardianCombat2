//=================================================================================-
//
//		ポーズ選択制御
//
//=================================================================================
#pragma once

class Texture;
class PauseScene;
class PauseSetting;
class Sound;

#include <string>

class PauseSelect
{
public:
	enum WORDS
	{
		PAUSE = -1,
		MOUSE,
		CONTROLLER,
		BACKGAME,
		BACKTITLE,
		WORD_MAX
	};

	struct Position
	{
		float x;
		float y;
	};
	struct Size
	{
		float x;
		float y;
	};

	PauseSelect(PauseScene* scene);
	~PauseSelect();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ClosePause();					//Pause閉じる際に呼び出し
	inline void Selecting() { isSelecting_ = true; }		//選択中
	bool IsHitMouseBackGameWord();													//ゲームに戻る文字にマウスが当たっているか
	bool IsHitMouseBackTitleWord();													//タイトルに戻る文字にマウスが当たっているか
	inline void SetEnableClick(bool flag) { isEnableClick_ = flag; }		//クリックによる選択可能かを設定
private:
	void SaveSettings(std::string filename);				//設定した情報をファイル書き込み
	void LoadSettings(std::string filename);				//ファイルに出力した情報を読み込む

	PauseScene* pauseScene_ = nullptr;				//ポーズシーン
	PauseSetting* pauseSetting_ = nullptr;				//ポーズ設定制御
	bool isEnableClick_ = true;									//マウスクリックによる選択が可能か
	bool isSelecting_ = true;										//デバイス選択中か
	float stayColor_[3] = { 0.0f };;								//通常時文字色
	float selectingColor_[3] = { 0.0f };;						//選択時文字色
	Texture* selectBack_ = nullptr;							//選択中の背景テクスチャ
	Texture* pauseWord_ = nullptr;							//ポーズ文字
	Texture* mouseWord_ = nullptr;							//マウス文字
	Texture* backGame_ = nullptr;							//ゲームに戻る文字
	Texture* controllerWord_ = nullptr;					//コントローラー文字
	Texture* backTitle_ = nullptr;								//タイトルに戻る文字
	Size selectBackSize_ = { 0.0f };							//選択中の拝啓大きさ
	float selectBackColor_[4] = { 0.0f };					//選択中の背景いろ
	Position pausePos_ = { 0.0f };								//ポーズ文字座標
	Size pauseSize_ = { 0.0f };									//ポーズ文字大きさ
	Position mousePos_ = { 0.0f };							//マウス文字座標
	Size mouseSize_ = { 0.0f };									//マウス文字大きさ
	Position controllerPos_ = { 0.0f };						//コントローラ文字座標
	Size controllerSize_ = { 0.0f };								//コントローラ文字大きさ
	Position backGamePos_ = { 0.0f };						//ゲームに戻る文字座標
	Size backGameSize_ = { 0.0f };							//ゲームに戻る文字サイズ
	Position backTitlePos_ = { 0.0f };						//タイトルに戻る文字座標
	Size backTitleSize_ = { 0.0f };								//タイトルに戻る文字サイズ
	int selectingIndex_ = MOUSE;							//選択中のインデックス
	Sound* selectingSE_ = nullptr;							//選択中SE
	Sound* selectSE_ = nullptr;								//選択SE
};	
