#pragma once
//==========================================================================
//
//		スキル選択シーンのUI制御
//
//===========================================================================

class Texture;
class SelectSkillEffect;
class SelectSkillScene;

class SelectSkillUI
{
public:
	SelectSkillUI(SelectSkillScene*scene);
	~SelectSkillUI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void ResetIconColor(int skill_index);

private:
	SelectSkillScene* scene_ = nullptr;			//シーン情報
	Texture* back_ = nullptr;							//背景テクスチャ
	Texture* iconDash_ = nullptr;						//ダッシュアイコン
	Texture* iconHeal_ = nullptr;						//ヒールアイコン
	Texture* iconThunder_ = nullptr;				//サンダーアイコン
	Texture* iconSpeedUp_ = nullptr;				//スピードアップアイコン
	Texture* expDash_ = nullptr;						//ダッシュ説明テクスチャ
	Texture* expHeal_ = nullptr;						//ヒール説明テクスチャ
	Texture* expThunder_ = nullptr;					//サンダー説明テクスチャ
	Texture* expSpeedUp_ = nullptr;				//スピードアップ説明テクスチャ
};