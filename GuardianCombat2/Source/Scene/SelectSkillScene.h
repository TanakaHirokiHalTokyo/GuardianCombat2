#pragma once
//=========================================================================
//
//		スキル選択シーン
//
//=========================================================================

#include "Scene.h"

class CEffekseer;
class Texture;

class SelectSkillScene:public Scene
{
public:
	SelectSkillScene();
	~SelectSkillScene()override;
	void Init()override; 
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	CEffekseer* effectHeal_ = nullptr;							//ヒールエフェクト
	CEffekseer* effectDash_ = nullptr;							//ダッシュエフェクト
	CEffekseer* effectSpeedUp_ = nullptr;					//スピードアップエフェクト
	CEffekseer* effectThunder_ = nullptr;					//雷エフェクト

};