#pragma once
//==============================================================
//
//		TitleSelect �����𐧌�
//		EditMode�ɂ����̂�GameScene�ɂ����̂��Q�[���I�������f
//==============================================================

class Texture;
class Sound;
class TitleScene;

class TitleSelect
{
public:
	enum SELECT_MODE
	{
		EDIT = 0,						//�p�����[�^�ҏW���
		GAME,							//�Q�[�����[�h
		EXIT,							//�I��
		MODE_MAX,
	};

	TitleSelect(TitleScene* scene);			//�R���X�g���N�^
	~TitleSelect();				//�f�X�g���N�^
	void Init();						//����������
	void Uninit();					//�I������
	void Update();				//�X�V����
	void Draw();					//�`�揈��

	inline int GetSelectMode() { return selectMode_; }			//���ݑI�𒆂̃��[�h���擾
private:
	TitleScene* titleScene_ = nullptr;					//�^�C�g���V�[��
	Sound* selectingSE_ = nullptr;						//�I�𒆂̌��ʉ�
	int selectMode_ = (int)EDIT;							//�I�𒆃��[�h
	Texture* edit_ = nullptr;								//Edit�e�N�X�`���|�C���^
	Texture* game_ = nullptr;								//Game�e�N�X�`���|�C���^
	Texture* exit_ = nullptr;								//Exit�e�N�X�`���|�C���^
	float scaleRate_ = 1.0f;									//�X�P�[���䗦
	int controllerCounter_ = 0;							//�X�e�B�b�N����J�E���^
	bool isActiveControllerStick_ = true;				//�X�e�B�b�N����󂯕t���邩
	bool isClicked_ = false;
};