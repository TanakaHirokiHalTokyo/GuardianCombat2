//========================================================================
//
//		Tutorialシーン
//
//========================================================================
#pragma once

#include "Scene.h"
#include <string>

class PauseScene;
class Sound;
class Player;
class EnemyHige;
class Texture;

class Tutorial :public Scene
{
public:
	Tutorial();
	~Tutorial()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	//チュートリアル中点滅速度
	const float TUTORIAL_WORD_ALPHA_SPEED = 3.0f;

	//UI用
	void UpdateUI();											//UI用の更新
	void DrawUI();												//UI用の描画
	void UpdateTutorialWord();							//チュートリアル中の文字更新
	void SaveUISettings(std::string filename);			//UI用の設定を保存
	void LoadUISettings(std::string filename);			//UI用の設定を読込

	PauseScene* pauseScene_ = nullptr;			//ポーズシーン
	Sound* bgm_ = nullptr;									//BGM
	Player* player_ = nullptr;								//Player
	EnemyHige* enemy_ = nullptr;						//Enemy
	Texture* tutorialWord_ = nullptr;					//チュートリアル文字
	Texture* explainTrans_ = nullptr;					//移動説明テクスチャ
	Texture* explainMouse_ = nullptr;					//マウス説明テクスチャ
	Texture* finishTutorial_ = nullptr;					//チュートリアル終了文字
	bool playGame_ = false;									//ゲーム開始するか

	float tutorialFinishPos_[2] = { 0.0f };				//チュートリアル終了文字
	float tutorialFinishSize_[2] = { 0.0f };				//チュートリアル終了文字サイズ
	float tutorialWordPos_[2] = { 0.0f };				//チュートリアル文字座標
	float tutorialWordSize_[2] = { 0.0f };				//チュートリアル文字サイズ
	float explainTransPos_[2] = { 0.0f };				//移動説明座標
	float explainTransSize_[2] = { 0.0f };				//移動説明サイズ
	float explainMousePos_[2] = { 0.0f };				//マウス説明座標
	float explainMouseSize_[2] = { 0.0f };			//マウス説明サイズ

	float tutorialWordAlpha_ = 255.0f;					//チュートリアル中透明度
	float tutorialWordAlphaSpeed_ = -TUTORIAL_WORD_ALPHA_SPEED;		//チュートリアル中点滅速度
};
