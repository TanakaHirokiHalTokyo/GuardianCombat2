//============================================================================================
//
//		�Q�[�����ʃV�[��
//
//============================================================================================
#pragma once
#include "Scene.h"
#include <string>

class Texture;
class Sound;

class GameResult :public Scene
{
public:
	GameResult();
	~GameResult()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	void SaveSettings(std::string filename);
	void LoadSettings(std::string filename);
	
	int selectResultNumber_ = 0;				//�I�𒆂̌��ʔԍ� 0: Win 1 : Lose
	Sound* bgm_ = nullptr;						//BGM
	Sound* selectSE_ = nullptr;				//�I�����ʉ�
	Texture* pressKey_ = nullptr;				//PressKey�e�N�X�`��
	Texture* resultWord_ = nullptr;			//���U���g���ʕ���
	Texture* resultBack_ = nullptr;			//���U���g�w�i
	float keyColor_[3] = { 0 };					//PressKey�F
	float keyAlpha_ = 0.0f;						//PressKeyAlpha
	float wordWinColor_[3] = {0};				//���������F
	float wordLoseColor_[3] = { 0 };			//�s�k�����F
	float backWinColor_[3] = {0};				//�����w�i�F
	float backLoseColor_[3] = { 0 };			//�s�k�w�i�F
	float scaleAddRate_ = 0.002f;				//�X�P�[���l�g�嗦
	float scaleRate_ = 1.0f;						//�X�P�[���l
	float alphaAddRate_ = 1.5f;				//���l���Z�l
};