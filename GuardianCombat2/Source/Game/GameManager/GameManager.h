#pragma once

#include "../../Scene/Scene.h"
#include <string>

class Fade;
class Scene;
class Player;
class DirectionalLight;

class GameManager
{
public:
	
	static void Init();													//������
	static void Uninit();												//�I������
	static void Update();												//�X�V����
	static void Draw();													//�`�揈��

	static void SetScene(Scene* scene);									//�V�����V�[����ݒ�
	static Scene* GetScene();											//�V�[�����擾
	static Fade* GetFade();												//�t�F�[�h���擾
	static void SetDirectionalLight(DirectionalLight* light);			//�f�B���N�V���i�����C�g�ݒ�
	static DirectionalLight* GetDirectionalLight();						//�f�B���N�V���i�����C�g�擾
	static void SetSceneTag(std::string sceneTag);						//�V�[���^�O�ݒ�
	static std::string GetSceneTag();									//�V�[���^�O���擾
	static void SetGameObjectLoad(bool loaded);							//GameObject�����łɓǂݍ���ł���̂���ݒ�B
	static bool GetGameObjectLoad();									//GameObject�����łɓǂݍ���ł���̂����擾�B

	static void SetPlayer(Player* player);								//Player���o�^�B
	static Player* GetPlayer();											//Player���擾�B
private:
	static Scene* scene_;												//Scene���
	static Fade* fade_;													//Fade���
	static DirectionalLight* light_;									//DirectionalLight���
	static std::string sceneTag_;										//���݂̃V�[���^�O
	static bool gameObjectLoaded_;										//GameObject�����łɃ��[�h�ς݂�
	static Player* player_;												//�v���C���[���
};