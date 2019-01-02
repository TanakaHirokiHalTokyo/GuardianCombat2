#pragma once
//==============================================================
//
//		TitleSelect 動きを制御
//		EditModeにいくのかGameSceneにいくのかゲーム終了か判断
//==============================================================

class Texture;


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

	TitleSelect();				//コンストラクタ
	~TitleSelect();				//デストラクタ
	void Init();					//初期化処理
	void Uninit();				//終了処理
	void Update();				//更新処理
	void Draw();					//描画処理

	inline int GetSelectMode() { return selectMode_; }			//現在選択中のモードを取得
private:
	int selectMode_ = (int)EDIT;						//選択中モード
	Texture* edit_ = nullptr;								//Editテクスチャポインタ
	Texture* game_ = nullptr;							//Gameテクスチャポインタ
	Texture* exit_ = nullptr;								//Exitテクスチャポインタ
	float scaleRate_ = 1.0f;								//スケール比率
};