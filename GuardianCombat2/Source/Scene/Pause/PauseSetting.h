//===================================================================
//
//		PauseSetting �}�E�X�E�R���g���[�����x�ύX�Ȃ�
//
//===================================================================
#pragma once

class Texture;
class DeviceConfig;
class PauseSelect;

#include <string>

class PauseSetting
{
public:
	static const std::string SENCITIVITY_FILENAME;

	enum DEVICE
	{
		MOUSE = 1,
		CONTROLLER
	};
	struct Size
	{
		float x;
		float y;
	};
	struct Position
	{
		float x;
		float y;
	};

	PauseSetting(PauseSelect* select);
	~PauseSetting();
	void Init();
	void Uninit();
	void Update();
	void Draw();
	inline void ChangeDevice(DEVICE dev) { device_ = dev; }			//�I���f�o�C�X�ύX
	void SetCirclePosition();			//�I�������f�o�C�X�̊��x���������ƂɊۂ̍��W�ݒ�

	void SaveSensitivity(std::string filename);		//���x���ۑ�
private:
	void SaveSettings(std::string filename);			//�ݒ肵���l�ۑ�
	void LoadSettings(std::string filename);			//�ݒ肵���l�Ǎ�
	void LoadSensitivity(std::string filename);		//���x���Ǎ�

	float mouse_rate_ = 1.0f;						//�}�E�X���x����
	float controller_rate_ = 1.0f;						//�R���g���[�����x����
	PauseSelect* select_ = nullptr;				//Select���
	DeviceConfig* config_ = nullptr;				//�f�o�C�X���x���
	DEVICE device_ = MOUSE;					//�I�𒆂̃f�o�C�X
	Texture* bar_ = nullptr;							//�X���C�_�[�o�[
	Texture* circle_ = nullptr;						//�ۃe�N�X�`��
	Texture* b_ = nullptr;								//B�e�N�X�`��
	Texture* back_ = nullptr;						//Back�e�N�X�`��
	Position bPosition_ = { 0.0f };					//B�̍��W
	Size bSize_ = { 0.0f };								//B�̃T�C�Y
	float bColor_[3] = { 0 };							//B�̐F
	Position backPosition_ = { 0.0f };			//Back�̍��W
	Size backSize_ = { 0.0f };					//Back�̃T�C�Y
	float backColor_[3] = { 0 };						//Back�̐F
	Position barPosition_ = { 0.0f };				//�o�[�̍��W
	Size barSize_ = { 0.0f };							//�o�[�̑傫��
	float barColor_[3] = { 0 };						//�o�[�̐F
	Position circlePosition_ = { 0.0f };			//�ۂ̍��W
	Size circleSize_ = { 0.0f };						//�ۂ̑傫��
	float circleColor_[3] = { 0 };					//�ۂ̐F
	bool isClicked_ = false;							//�N���b�N���ă{�^���𓮂����Ă���̂�
	float mouseStartX_ = 0.0f;						//�N���b�N�������_�ł̃}�E�XX���W
};
