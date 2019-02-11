#pragma once
//===========================================================================
//
//		スキル選択の背景エフェクト制御
//
//===========================================================================
class SelectSkillScene;
class Camera;
class CEffekseer;

class SelectSkillEffect
{
public:
	SelectSkillEffect(SelectSkillScene* scene);
	~SelectSkillEffect();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	SelectSkillScene* scene_ = nullptr;				//シーン情報
	Camera* camera_ = nullptr;							//カメラ情報
	CEffekseer* efDash_ = nullptr;						//ダッシュエフェクト
	CEffekseer* efHeal_ = nullptr;						//ヒールエフェクト
	CEffekseer* efThunder_ = nullptr;					//サンダーエフェクト
	CEffekseer* efSpeedUp_ = nullptr;					//スピードアップエフェクト
};