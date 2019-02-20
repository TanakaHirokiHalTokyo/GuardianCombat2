#pragma once

#include "../../Scene/Scene.h"
#include <string>

class Fade;
class Scene;
class Player;
class DirectionalLight;
class Blur;

class GameManager
{
public:
	
	static void Init();													//初期化
	static void Uninit();												//終了処理
	static void Update();											//更新処理
	static void Draw();												//描画処理

	static void SetScene(Scene* scene);											//新しいシーンを設定
	static Scene* GetScene();															//シーンを取得
	static Fade* GetFade();																//フェード情報取得
	static void SetDirectionalLight(DirectionalLight* light);			//ディレクショナルライト設定
	static DirectionalLight* GetDirectionalLight();							//ディレクショナルライト取得
	static void SetSceneTag(std::string sceneTag);							//シーンタグ設定
	static std::string GetSceneTag();												//シーンタグを取得

	static void GameOver(bool flag);												//ゲームが終了したとき
	static inline bool GetGameOver() { return gameOver_; }			//ゲームオーバーしたか

	static inline void ResetEnding() { ending_ = false; }					//エンディングフラグ初期化
	static inline void StartEnding() { ending_ = true; }						//エンディング開始
	static inline bool GetEnding() { return ending_; }						//エンディング中かを取得

	static inline void SetGameClear(bool flag) { gameClear_ = flag; }			//ゲームクリアフラグ設定
	static inline bool IsGameClear() { return gameClear_; }							//ゲームクリアフラグ取得

	static Player* SetPlayer(Player* player);									//Player情報登録。
	static Player* GetPlayer();															//Player情報取得。

	static inline void StartGame() { playingGame_ = true; }												//ゲーム開始時呼び出し
	static inline void EndGame() { playingGame_ = false; enableEdit_ = true; }				//ゲーム終了時呼び出し

	static inline bool GetEnableEdit() { return enableEdit_; }					//編集画面表示させるかフラグを取得

	static inline void SetPlayerSkill1(int index) { playerSkill1_ = index; }	//Playerの1つ目のスキルセット
	static inline void SetPlayerSkill2(int index) { playerSkill2_ = index; }	//Playerの2つ目のスキルセット
	static inline int GetPlayerSkill1() { return playerSkill1_; }						//Playerの1つ目のスキル取得
	static inline int GetPlayerSkill2() { return playerSkill2_; }						//Playerの2つ目のスキル取得

	static inline void SetReturnTitle(bool flag) { returnTitle_ = flag; }			//タイトルに戻るかのフラグを設定
	static inline bool GetReturnTitle() { return returnTitle_; }						//タイトルに戻るかのフラグを取得

	static inline void SetGamePause(bool flag) { isGamePausing_ = flag; }	//ポーズフラグ設定
	static inline bool GetGamePause() { return isGamePausing_; }					//ポーズフラグ取得

	static Blur* GetBlur();												//Blur情報取得
private:
	static Scene* scene_;												//Scene情報
	static Fade* fade_;													//Fade情報
	static DirectionalLight* light_;								//DirectionalLight情報
	static std::string sceneTag_;									//現在のシーンタグ
	static Player* player_;												//プレイヤー情報
	static Blur* blur_;													//ブラー情報
	static bool enableEdit_;											//編集を可能にするか
	static bool playingGame_;										//ゲームプレイ中か
	static bool gameClear_;											//ゲームクリアフラグ
	static bool gameOver_;											//ゲームオーバーフラグ
	static bool ending_;													//エンディングフラグ
	static int playerSkill1_;											//PlayerSkill
	static int playerSkill2_;											//PlayerSkill
	static bool returnTitle_;											//タイトルに戻るか
	static bool isGamePausing_;									//ゲームポーズ中か
};