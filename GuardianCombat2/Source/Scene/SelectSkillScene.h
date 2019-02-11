#pragma once
//=========================================================================
//
//		スキル選択シーン
//
//=========================================================================

#include "Scene.h"

class SelectSkillUI;
class SelectSkillEffect;
class Fade;

class SelectSkillScene:public Scene
{
public:
	enum SKILL_NUMBER			//スキル番号
	{
		SKILL_DASH = 0,				//ダッシュ
		SKILL_HEAL,						//ヒール
		SKILL_THUDER,					//サンダー
		SKILL_SPEEDUP,				//スピードアップ
		SKILL_MAX
	};

	SelectSkillScene();
	~SelectSkillScene()override;
	void Init()override; 
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	inline int GetSelectingNumber() { return selectingNumber_; }				//現在選択中のスキル取得
	inline int GetSelectingOldNumber() { return selectingOldNumber_; }		//前フレーム選択していたスキル取得
	inline int GetSelectedSkill1() { return selectedSkill1_; }							//選択したスキル１取得
	inline int GetSelectedSkill2() { return selectedSkill2_; }							//選択したスキル２取得

private:
	SelectSkillUI* uiSystem_ = nullptr;				//UIを管理するクラス
	SelectSkillEffect* efSystem_ = nullptr;			//エフェクトを管理するクラス
	Fade* fade_ = nullptr;									//フェードポインタ
	int selectingOldNumber_ = 0;						//前回選択していたスキル番号
	int selectingNumber_ = 0;								//現在選択中のスキルの番号
	int selectedSkill1_ = -1;									//1つ目の選択したスキル初期値ー１
	int selectedSkill2_ = -1;									//2爪の選択したスキル初期値－１
	int controllerCounter_ = 0;							//スティック制御カウンタ
	bool isActiveControllerStick_ = true;				//スティック制御受け付けるか
};