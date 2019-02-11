//=================================================================================-
//
//		�|�[�Y�I�𐧌�
//
//=================================================================================
#pragma once

class Texture;
class PauseScene;
class PauseSetting;
class Sound;

#include <string>

class PauseSelect
{
public:
	enum WORDS
	{
		PAUSE,
		MOUSE,
		CONTROLLER,
		WORD_MAX
	};

	struct Position
	{
		float x;
		float y;
	};
	struct Size
	{
		float x;
		float y;
	};

	PauseSelect();
	~PauseSelect();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void ClosePause();					//Pause����ۂɌĂяo��
	inline void Selecting() { isSelecting_ = true; }		//�I��
	bool IsHitMouseBackGameWord();							//�Q�[���ɖ߂镶���Ƀ}�E�X���������Ă��邩
	bool IsHitMouseBackTitleWord();							//�^�C�g���ɖ߂镶���Ƀ}�E�X���������Ă��邩
	inline void SetEnableClick(bool flag) { isEnableClick_ = flag; }		//�N���b�N�ɂ��I���\����ݒ�
private:
	void SaveSettings(std::string filename);				//�ݒ肵�������t�@�C����������
	void LoadSettings(std::string filename);				//�t�@�C���ɏo�͂�������ǂݍ���

	bool isEnableClick_ = true;									//�}�E�X�N���b�N�ɂ��I�����\��
	bool isSelecting_ = true;										//�f�o�C�X�I�𒆂�
	PauseSetting* pauseSetting_ = nullptr;		//�|�[�Y�ݒ萧��
	float stayColor_[3];												//�ʏ펞�����F
	float selectingColor_[3];										//�I���������F
	Texture* pauseWord_ = nullptr;							//�|�[�Y����
	Texture* mouseWord_ = nullptr;							//�}�E�X����
	Texture* backGame_ = nullptr;							//�Q�[���ɖ߂镶��
	Texture* controllerWord_ = nullptr;					//�R���g���[���[����
	Texture* backTitle_ = nullptr;								//�^�C�g���ɖ߂镶��
	Position pausePos_ = { 0.0f };								//�|�[�Y�������W
	Size pauseSize_ = { 0.0f };									//�|�[�Y�����傫��
	Position mousePos_ = { 0.0f };							//�}�E�X�������W
	Size mouseSize_ = { 0.0f };									//�}�E�X�����傫��
	Position controllerPos_ = { 0.0f };						//�R���g���[���������W
	Size controllerSize_ = { 0.0f };								//�R���g���[�������傫��
	Position backGamePos_ = { 0.0f };						//�Q�[���ɖ߂镶�����W
	Size backGameSize_ = { 0.0f };							//�Q�[���ɖ߂镶���T�C�Y
	float backGameColor_[3] = { 0.0f };					//�Q�[���ɖ߂镶���F
	Position backTitlePos_ = { 0.0f };						//�^�C�g���ɖ߂镶�����W
	Size backTitleSize_ = { 0.0f };								//�^�C�g���ɖ߂镶���T�C�Y
	float backTitleColor_[3] = { 0.0f };						//�^�C�g���ɖ߂镶���F
	int selectingIndex_ = MOUSE;							//�I�𒆂̃C���f�b�N�X
	Sound* selectingSE_ = nullptr;							//�I��SE
	Sound* selectSE_ = nullptr;								//�I��SE
};	
