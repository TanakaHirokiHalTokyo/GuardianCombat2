#pragma once

//=========================================================
//
//		�u���[
//
//=========================================================

class Texture;

class Blur
{
public:
	Blur();													//�R���X�g���N�^
	~Blur();												//�f�X�g���N�^
	void BeginDraw();								//�ʏ�̕`�揈���̑O�ɋ���
	void EndDraw();									//�ʏ�̕`�揈�����I������ۂɂ��鏈��

	void SetUseBlur(bool flag);					//�u���[���g�����̃Z�b�g
	bool GetUseBlur();								//�u���[���g���Ă��邩�̎擾
	void SetAlpha(int value);						//�u���[�e�N�X�`���̂ڂ�����ݒ�
	void SetStrength(float value);				//�u���[�̋����ݒ�
private:
	bool useBlur_ = false;							//���݃u���[���g���Ă���̂�
	Texture* blurTex_ = nullptr;				//�u���[�e�N�X�`��
	int alpha_ = 150;									//�u���[�e�N�X�`����
	float strength_ = 1.0f;							//�u���[�̋���
};