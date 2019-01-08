#pragma once
//=========================================================================
//
//		�X�L���I���V�[��
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
	CEffekseer* effectHeal_ = nullptr;							//�q�[���G�t�F�N�g
	CEffekseer* effectDash_ = nullptr;							//�_�b�V���G�t�F�N�g
	CEffekseer* effectSpeedUp_ = nullptr;					//�X�s�[�h�A�b�v�G�t�F�N�g
	CEffekseer* effectThunder_ = nullptr;					//���G�t�F�N�g

};