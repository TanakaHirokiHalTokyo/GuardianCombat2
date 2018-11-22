#pragma once
//==========================================================
//
//		�G�i�Ђ��j�̕��g
//
//==========================================================
#include "Enemy.h"

class StateEnemy;
class EnemyHige;

class EnemyHige_Avater :public Enemy
{
public:
	EnemyHige_Avater();															//�R���X�g���N�^
	~EnemyHige_Avater();														//�f�X�g���N�^
	void Init()override;																//����������
	void Uninit()override;															//�I������
	void Update()override;														//�X�V����
	void BeginDraw()override;													//���O�`�揈��
	void Draw()override;															//�`�揈��
	void EndDraw()override;														//�I���`�揈��

	void SetSpawnVector(D3DXVECTOR3 vec);						//�x�N�g���ݒ�
	void SetSpawnVector(float x,float y,float z);						//�x�N�g���ݒ�
	D3DXVECTOR3 GetSpawnVector();									//�X�|�[���x�N�g���x�N�g���擾
	void SetMoving(bool flag) { moving_ = flag; }			//�ړ����Ă���̂���ݒ�
	bool GetMoving() { return moving_; }						//�ړ����Ă���̂����擾
private:
	StateEnemy* rotate_ = nullptr;											//��]����
	D3DXVECTOR3 spawnVec_ = D3DXVECTOR3(0,0,0);		//�X�|�[���x�N�g��
	float distance_ = 0.0f;															//�i�񂾋���
	bool moving_ = true;															//�����Ă���̂�
};