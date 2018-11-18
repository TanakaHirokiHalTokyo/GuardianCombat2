#pragma once
//=======================================================================
//
//		�e���|�[�g�@����
//
//=======================================================================
#include "StateAction.h"
class StateEnemyHige;

class StateAction_Teleportation :public StateAction
{
public:
	StateAction_Teleportation(Enemy* enemy);
	~StateAction_Teleportation()override;
	void Action()override;									//���s�֐�
	void BeginDisplay()override;						//���O�\��
	void Display()override;								//�\��
	void EndDisplay()override;							//�\���I��
private:
	StateEnemyHige*  teleportation_ = nullptr;
};