//========================================================================
//
//		Tutorial�V�[��
//
//========================================================================
#pragma once

#include "Scene.h"
#include <string>

class PauseScene;
class Sound;
class Player;
class EnemyHige;
class Texture;

class Tutorial :public Scene
{
public:
	Tutorial();
	~Tutorial()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	//�`���[�g���A�����_�ő��x
	const float TUTORIAL_WORD_ALPHA_SPEED = 3.0f;

	//UI�p
	void UpdateUI();											//UI�p�̍X�V
	void DrawUI();												//UI�p�̕`��
	void UpdateTutorialWord();							//�`���[�g���A�����̕����X�V
	void SaveUISettings(std::string filename);			//UI�p�̐ݒ��ۑ�
	void LoadUISettings(std::string filename);			//UI�p�̐ݒ��Ǎ�

	PauseScene* pauseScene_ = nullptr;			//�|�[�Y�V�[��
	Sound* bgm_ = nullptr;									//BGM
	Player* player_ = nullptr;								//Player
	EnemyHige* enemy_ = nullptr;						//Enemy
	Texture* tutorialWord_ = nullptr;					//�`���[�g���A������
	Texture* explainTrans_ = nullptr;					//�ړ������e�N�X�`��
	Texture* explainMouse_ = nullptr;					//�}�E�X�����e�N�X�`��
	Texture* finishTutorial_ = nullptr;					//�`���[�g���A���I������
	bool playGame_ = false;									//�Q�[���J�n���邩

	float tutorialFinishPos_[2] = { 0.0f };				//�`���[�g���A���I������
	float tutorialFinishSize_[2] = { 0.0f };				//�`���[�g���A���I�������T�C�Y
	float tutorialWordPos_[2] = { 0.0f };				//�`���[�g���A���������W
	float tutorialWordSize_[2] = { 0.0f };				//�`���[�g���A�������T�C�Y
	float explainTransPos_[2] = { 0.0f };				//�ړ��������W
	float explainTransSize_[2] = { 0.0f };				//�ړ������T�C�Y
	float explainMousePos_[2] = { 0.0f };				//�}�E�X�������W
	float explainMouseSize_[2] = { 0.0f };			//�}�E�X�����T�C�Y

	float tutorialWordAlpha_ = 255.0f;					//�`���[�g���A���������x
	float tutorialWordAlphaSpeed_ = -TUTORIAL_WORD_ALPHA_SPEED;		//�`���[�g���A�����_�ő��x
};
