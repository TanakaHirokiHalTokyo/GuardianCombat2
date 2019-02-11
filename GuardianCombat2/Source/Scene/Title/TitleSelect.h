#pragma once
//==============================================================
//
//		TitleSelect 動きを制御
//		EditModeにいくのかGameSceneにいくのかゲーム終了か判断
//==============================================================

class Texture;
class Sound;
class TitleScene;

class TitleSelect
{
public:
	enum SELECT_MODE
	{
		EDIT = 0,						//パラメータ編集画面
		GAME,							//ゲームモード
		EXIT,							//終了
		MODE_MAX,
	};

	TitleSelect(TitleScene* scene);			//コンストラクタ
	~TitleSelect();				//デストラクタ
	void Init();						//初期化処理
	void Uninit();					//終了処理
	void Update();				//更新処理
	void Draw();					//描画処理

	inline int GetSelectMode() { return selectMode_; }			//現在選択中のモードを取得
private:
	TitleScene* titleScene_ = nullptr;					//タイトルシーン
	Sound* selectingSE_ = nullptr;						//選択中の効果音
	int selectMode_ = (int)EDIT;							//選択中モード
	Texture* edit_ = nullptr;								//Editテクスチャポインタ
	Texture* game_ = nullptr;								//Gameテクスチャポインタ
	Texture* exit_ = nullptr;								//Exitテクスチャポインタ
	float scaleRate_ = 1.0f;									//スケール比率
	int controllerCounter_ = 0;							//スティック制御カウンタ
	bool isActiveControllerStick_ = true;				//スティック制御受け付けるか
	bool isClicked_ = false;
};