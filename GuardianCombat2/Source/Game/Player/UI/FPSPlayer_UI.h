#pragma once

//==========================================================
//
//		FPSPlayer ���[�U�[�C���^�[�t�F�[�X
//
//==========================================================

class Texture;
class Player;

class FPSPlayer_UI
{
public:
	FPSPlayer_UI();
	~FPSPlayer_UI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	Player* player_ = nullptr;						//Player�|�C���^
	Texture* lifeBar_ = nullptr;						//���C�t�o�[
	Texture* lifeBarMax_ = nullptr;				//���C�t�o�[�̔w�i
	Texture* reticle_ = nullptr;						//���e�B�N���e�N�X�`��
	Texture* damage_ = nullptr;					//DamageScreen
};