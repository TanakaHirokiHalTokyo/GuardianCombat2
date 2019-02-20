#pragma once

#include "../../Scene/Scene.h"
#include <string>

class Fade;
class Scene;
class Player;
class DirectionalLight;
class Blur;

class GameManager
{
public:
	
	static void Init();													//������
	static void Uninit();												//�I������
	static void Update();											//�X�V����
	static void Draw();												//�`�揈��

	static void SetScene(Scene* scene);											//�V�����V�[����ݒ�
	static Scene* GetScene();															//�V�[�����擾
	static Fade* GetFade();																//�t�F�[�h���擾
	static void SetDirectionalLight(DirectionalLight* light);			//�f�B���N�V���i�����C�g�ݒ�
	static DirectionalLight* GetDirectionalLight();							//�f�B���N�V���i�����C�g�擾
	static void SetSceneTag(std::string sceneTag);							//�V�[���^�O�ݒ�
	static std::string GetSceneTag();												//�V�[���^�O���擾

	static void GameOver(bool flag);												//�Q�[�����I�������Ƃ�
	static inline bool GetGameOver() { return gameOver_; }			//�Q�[���I�[�o�[������

	static inline void ResetEnding() { ending_ = false; }					//�G���f�B���O�t���O������
	static inline void StartEnding() { ending_ = true; }						//�G���f�B���O�J�n
	static inline bool GetEnding() { return ending_; }						//�G���f�B���O�������擾

	static inline void SetGameClear(bool flag) { gameClear_ = flag; }			//�Q�[���N���A�t���O�ݒ�
	static inline bool IsGameClear() { return gameClear_; }							//�Q�[���N���A�t���O�擾

	static Player* SetPlayer(Player* player);									//Player���o�^�B
	static Player* GetPlayer();															//Player���擾�B

	static inline void StartGame() { playingGame_ = true; }												//�Q�[���J�n���Ăяo��
	static inline void EndGame() { playingGame_ = false; enableEdit_ = true; }				//�Q�[���I�����Ăяo��

	static inline bool GetEnableEdit() { return enableEdit_; }					//�ҏW��ʕ\�������邩�t���O���擾

	static inline void SetPlayerSkill1(int index) { playerSkill1_ = index; }	//Player��1�ڂ̃X�L���Z�b�g
	static inline void SetPlayerSkill2(int index) { playerSkill2_ = index; }	//Player��2�ڂ̃X�L���Z�b�g
	static inline int GetPlayerSkill1() { return playerSkill1_; }						//Player��1�ڂ̃X�L���擾
	static inline int GetPlayerSkill2() { return playerSkill2_; }						//Player��2�ڂ̃X�L���擾

	static inline void SetReturnTitle(bool flag) { returnTitle_ = flag; }			//�^�C�g���ɖ߂邩�̃t���O��ݒ�
	static inline bool GetReturnTitle() { return returnTitle_; }						//�^�C�g���ɖ߂邩�̃t���O���擾

	static inline void SetGamePause(bool flag) { isGamePausing_ = flag; }	//�|�[�Y�t���O�ݒ�
	static inline bool GetGamePause() { return isGamePausing_; }					//�|�[�Y�t���O�擾

	static Blur* GetBlur();												//Blur���擾
private:
	static Scene* scene_;												//Scene���
	static Fade* fade_;													//Fade���
	static DirectionalLight* light_;								//DirectionalLight���
	static std::string sceneTag_;									//���݂̃V�[���^�O
	static Player* player_;												//�v���C���[���
	static Blur* blur_;													//�u���[���
	static bool enableEdit_;											//�ҏW���\�ɂ��邩
	static bool playingGame_;										//�Q�[���v���C����
	static bool gameClear_;											//�Q�[���N���A�t���O
	static bool gameOver_;											//�Q�[���I�[�o�[�t���O
	static bool ending_;													//�G���f�B���O�t���O
	static int playerSkill1_;											//PlayerSkill
	static int playerSkill2_;											//PlayerSkill
	static bool returnTitle_;											//�^�C�g���ɖ߂邩
	static bool isGamePausing_;									//�Q�[���|�[�Y����
};