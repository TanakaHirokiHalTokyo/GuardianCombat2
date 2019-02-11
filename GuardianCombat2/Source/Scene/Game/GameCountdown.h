#pragma once
//====================================================================
//
//		�Q�[���J�n�O�̃J�E���g�_�E���𐧌�
//
//====================================================================
class Texture;
class Sound;

class GameCountdown
{
public:
	GameCountdown();
	~GameCountdown();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	inline bool GetCountDown() { return isCountDown_; }			//�J�E���g�_�E�������擾
private:
	Sound* countSE_ = nullptr;			//�J�E���g���̌��ʉ�
	Sound* startSE_ = nullptr;				//�X�^�[�g���̌��ʉ�
	Texture* back_ = nullptr;				//�w�i�e�N�X�`��
	Texture* count1_ = nullptr;				//�J�E���g�P�e�N�X�`��
	Texture* count2_ = nullptr;				//�J�E���g�Q�e�N�X�`��
	Texture* count3_ = nullptr;				//�J�E���g�R�e�N�X�`��
	float wordSize_ = 0.0f;					//�����̃T�C�Y
	int index_ = 3;									//���ݕ\�����Ă��鐔��
	bool isCountDown_ = true;				//�J�E���g�_�E������
};