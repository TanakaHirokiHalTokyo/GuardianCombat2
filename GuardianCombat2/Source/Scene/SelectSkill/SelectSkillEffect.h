#pragma once
//===========================================================================
//
//		�X�L���I���̔w�i�G�t�F�N�g����
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
	SelectSkillScene* scene_ = nullptr;				//�V�[�����
	Camera* camera_ = nullptr;							//�J�������
	CEffekseer* efDash_ = nullptr;						//�_�b�V���G�t�F�N�g
	CEffekseer* efHeal_ = nullptr;						//�q�[���G�t�F�N�g
	CEffekseer* efThunder_ = nullptr;					//�T���_�[�G�t�F�N�g
	CEffekseer* efSpeedUp_ = nullptr;					//�X�s�[�h�A�b�v�G�t�F�N�g
};