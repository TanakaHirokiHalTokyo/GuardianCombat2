#pragma once

//==========================================================
//
//		FPSPlayer ���[�U�[�C���^�[�t�F�[�X
//
//==========================================================

class Texture;

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
	Texture* reticle_ = nullptr;		//���e�B�N���e�N�X�`��
};