#pragma once

#include "../../Scene/Scene.h"
#include <string>

class Fade;
class Scene;
class Player;
class DirectionalLight;

class GameManager
{
public:
	
	static void Init();													//初期化
	static void Uninit();												//終了処理
	static void Update();												//更新処理
	static void Draw();													//描画処理

	static void SetScene(Scene* scene);									//新しいシーンを設定
	static Scene* GetScene();											//シーンを取得
	static Fade* GetFade();												//フェード情報取得
	static void SetDirectionalLight(DirectionalLight* light);			//ディレクショナルライト設定
	static DirectionalLight* GetDirectionalLight();						//ディレクショナルライト取得
	static void SetSceneTag(std::string sceneTag);						//シーンタグ設定
	static std::string GetSceneTag();									//シーンタグを取得
	static void SetGameObjectLoad(bool loaded);							//GameObjectをすでに読み込んでいるのかを設定。
	static bool GetGameObjectLoad();									//GameObjectをすでに読み込んでいるのかを取得。

	static void SetPlayer(Player* player);								//Player情報登録。
	static Player* GetPlayer();											//Player情報取得。
private:
	static Scene* scene_;												//Scene情報
	static Fade* fade_;													//Fade情報
	static DirectionalLight* light_;									//DirectionalLight情報
	static std::string sceneTag_;										//現在のシーンタグ
	static bool gameObjectLoaded_;										//GameObjectをすでにロード済みか
	static Player* player_;												//プレイヤー情報
};