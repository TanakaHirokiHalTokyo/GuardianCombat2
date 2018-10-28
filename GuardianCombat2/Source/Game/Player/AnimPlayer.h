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
		bool rightRot[3] = { false,false,false };				//右回転させるか。
		bool min[3] = { false,false,false };					//最小角を取るか。
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
		UINT AnimNumber = 0;		//チェック中のアニメーション番号
		bool play = false;			//チェック中か
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
	void UpdatePart();						//パーツの更新処理
	void AnimationPlay();

	Part parts[10];
	bool animPlaying_ = false;				//アニメーション再生中か
	int nowFrame = 0;						//アニメーション再生時、フレームカウンタ
	UINT nowAnimKey_ = 0;					//アニメーション時、現在再生中のキー
	int frame_ = 0;							//アニメーション登録フレーム数
	D3DXVECTOR3 animRotate_[PART_MAX];		//1フレームあたりの回転角
	UINT keyNum_ = 0;						//アニメーションキーの数
	UINT viewKey_ = 0;						//現在見ているキー
	UINT selectType_ = (UINT)IDLE;			
	AnimData animData_[ANIM_KEYNUM];		//アニメーションデータ一時保存用
	Motion motion_[ANIMTYPE_MAX];
	AnimPlay animCheck_;
	bool typeChanged_ = false;				//アニメーションタイプをチェンジしたか
	bool walk = false;
	ANIM_TYPE animationType = IDLE;
};

