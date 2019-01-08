#pragma once
//==========================================================================
//
//		スキル選択シーンのUI制御
//
//===========================================================================

class Texture;

class SelectSkillUI
{
public:
	SelectSkillUI();
	~SelectSkillUI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	Texture* iconDash_ = nullptr;
	Texture* iconHeal_ = nullptr;
	Texture* iconThunder_ = nullptr;
	Texture* iconSpeedUp_ = nullptr;
	Texture* expDash_ = nullptr;
	Texture* expHeal_ = nullptr;
	Texture* expThunder_ = nullptr;
	Texture* expSpeedUp_ = nullptr;
};