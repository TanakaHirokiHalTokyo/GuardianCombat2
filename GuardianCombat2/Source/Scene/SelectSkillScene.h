#pragma once
//=========================================================================
//
//		�X�L���I���V�[��
//
//=========================================================================

#include "Scene.h"

class SelectSkillUI;
class SelectSkillEffect;
class Fade;

class SelectSkillScene:public Scene
{
public:
	enum SKILL_NUMBER			//�X�L���ԍ�
	{
		SKILL_DASH = 0,				//�_�b�V��
		SKILL_HEAL,						//�q�[��
		SKILL_THUDER,					//�T���_�[
		SKILL_SPEEDUP,				//�X�s�[�h�A�b�v
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

	inline int GetSelectingNumber() { return selectingNumber_; }				//���ݑI�𒆂̃X�L���擾
	inline int GetSelectingOldNumber() { return selectingOldNumber_; }		//�O�t���[���I�����Ă����X�L���擾
	inline int GetSelectedSkill1() { return selectedSkill1_; }							//�I�������X�L���P�擾
	inline int GetSelectedSkill2() { return selectedSkill2_; }							//�I�������X�L���Q�擾

private:
	SelectSkillUI* uiSystem_ = nullptr;				//UI���Ǘ�����N���X
	SelectSkillEffect* efSystem_ = nullptr;			//�G�t�F�N�g���Ǘ�����N���X
	Fade* fade_ = nullptr;									//�t�F�[�h�|�C���^
	int selectingOldNumber_ = 0;						//�O��I�����Ă����X�L���ԍ�
	int selectingNumber_ = 0;								//���ݑI�𒆂̃X�L���̔ԍ�
	int selectedSkill1_ = -1;									//1�ڂ̑I�������X�L�������l�[�P
	int selectedSkill2_ = -1;									//2�܂̑I�������X�L�������l�|�P
	int controllerCounter_ = 0;							//�X�e�B�b�N����J�E���^
	bool isActiveControllerStick_ = true;				//�X�e�B�b�N����󂯕t���邩
};