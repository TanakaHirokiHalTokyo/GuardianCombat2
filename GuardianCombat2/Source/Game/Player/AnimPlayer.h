#pragma once

#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include "../Object.h"

class XModel;

#define ANIM_KEYNUM		(30)

struct Transform
{
	D3DXVECTOR3 rotate;
	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
};

class AnimPlayer:public Object
{
public:
	struct AnimFile
	{
		char FileName[256];
	};

	enum ANIM_TYPE
	{
		IDLE,
		WALK,
		RUN,
		PUNCH1,
		ANIMTYPE_MAX,
	};
	

	const AnimFile animfile[ANIMTYPE_MAX] = {
		"resource/AnimationData/AnimData_Idle.bin",
		"resource/AnimationData/AnimData_Walk.bin",
		"resource/AnimationData/AnimData_Run.bin",
		"resource/AnimationData/AnimData_Punch1.bin",
	};

	enum PART_NAME
	{
		BODY = 0,
		HEAD,
		ARM_L,
		HAND_L,
		ARM_R,
		HAND_R,
		ASI_L,
		ASIKUBI_L,
		ASI_R,
		ASIKUBI_R,
		PART_MAX,
	};
	const char PartName[256][PART_MAX] = {
		"BODY",
		"HEAD",
		"ARM_L",
		"HAND_L",
		"ARM_R",
		"HAND_R",
		"ASI_L",
		"ASIKUBI_L",
		"ASI_R",
		"ASIKUBI_R",
	};

	struct Part
	{
		XModel* ModelInfo;
		D3DXMATRIX Matrix;
		Part* Parent;
	};
	struct AnimRotate
	{
		bool rightRot[3] = { false,false,false };				//�E��]�����邩�B
		bool min[3] = { false,false,false };					//�ŏ��p����邩�B
	};
	struct AnimData
	{
		Transform trans[PART_MAX];
		int frame = -1;
		AnimRotate animRot[PART_MAX];
	};
	struct Motion
	{
		AnimData animdata[ANIM_KEYNUM];
		UINT keynum = 0;
	};

	struct AnimPlay
	{
		UINT AnimNumber = 0;		//�`�F�b�N���̃A�j���[�V�����ԍ�
		bool play = false;			//�`�F�b�N����
	};

	AnimPlayer();
	~AnimPlayer();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	D3DXMATRIX GetMatrix(UINT Index);

private:
	void UpdatePart();						//�p�[�c�̍X�V����
	void AnimationPlay();

	Part parts[10];
	bool animPlaying_ = false;				//�A�j���[�V�����Đ�����
	int nowFrame = 0;						//�A�j���[�V�����Đ����A�t���[���J�E���^
	UINT nowAnimKey_ = 0;					//�A�j���[�V�������A���ݍĐ����̃L�[
	int frame_ = 0;							//�A�j���[�V�����o�^�t���[����
	D3DXVECTOR3 animRotate_[PART_MAX];		//1�t���[��������̉�]�p
	UINT keyNum_ = 0;						//�A�j���[�V�����L�[�̐�
	UINT viewKey_ = 0;						//���݌��Ă���L�[
	UINT selectType_ = (UINT)IDLE;			
	AnimData animData_[ANIM_KEYNUM];		//�A�j���[�V�����f�[�^�ꎞ�ۑ��p
	Motion motion_[ANIMTYPE_MAX];
	AnimPlay animCheck_;
	bool typeChanged_ = false;				//�A�j���[�V�����^�C�v���`�F���W������
	bool walk = false;
	ANIM_TYPE animationType = IDLE;
};

