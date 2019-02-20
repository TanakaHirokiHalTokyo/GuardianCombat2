//==========================================================
//
//			�w�肵���͈͓��������_���ɓ����G
//
//==========================================================
#pragma once

#include "../Object.h"

class XModel;
class Vector3;

class RandomMoveHige :public Object
{
public:
	struct Range
	{
		float min;
		float max;
	};

	RandomMoveHige();
	~RandomMoveHige()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;

	void Rotate();

	//		�͈͐ݒ�
	inline void SetRangeX(float min, float max) { rangeX_.min = min; rangeX_.max = max; }
	inline void SetRangeY(float min, float max) { rangeY_.min = min; rangeY_.max = max; }
	inline void SetRangeZ(float min, float max) { rangeZ_.min = min; rangeZ_.max = max; }
private:
	D3DXMATRIX world_;																//���[���h�s��
	XModel* model_ = nullptr;														//���f��
	Vector3* enemy_vector_ = nullptr;											//�G�̃x�N�g��
	Range rangeX_= { 0.0f };															//X�͈̔�
	Range rangeY_= { 0.0f };															//Y�͈̔�
	Range rangeZ_= { 0.0f };															//Z�͈̔�
	float length_ = 0.0f;																	//���̒n�_�܂ł̋���
	D3DXVECTOR3 next_position_ = D3DXVECTOR3(0,0,0);		//���̒n�_
	int timer_ = 0;																			//�ړI�n�_�t�߂܂ł����Ƃ����Ȃ������ꍇ�A���Ԍo�߂Ŏ��̒n�_�Z�o
};
