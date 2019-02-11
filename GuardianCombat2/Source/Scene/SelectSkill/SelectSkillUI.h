#pragma once
//==========================================================================
//
//		�X�L���I���V�[����UI����
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
	SelectSkillScene* scene_ = nullptr;			//�V�[�����
	Texture* back_ = nullptr;							//�w�i�e�N�X�`��
	Texture* iconDash_ = nullptr;						//�_�b�V���A�C�R��
	Texture* iconHeal_ = nullptr;						//�q�[���A�C�R��
	Texture* iconThunder_ = nullptr;				//�T���_�[�A�C�R��
	Texture* iconSpeedUp_ = nullptr;				//�X�s�[�h�A�b�v�A�C�R��
	Texture* expDash_ = nullptr;						//�_�b�V�������e�N�X�`��
	Texture* expHeal_ = nullptr;						//�q�[�������e�N�X�`��
	Texture* expThunder_ = nullptr;					//�T���_�[�����e�N�X�`��
	Texture* expSpeedUp_ = nullptr;				//�X�s�[�h�A�b�v�����e�N�X�`��
};