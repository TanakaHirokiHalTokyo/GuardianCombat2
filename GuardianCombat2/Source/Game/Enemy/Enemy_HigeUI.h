#pragma once

//==========================================================
//
//		Hige��UI�Ǘ�
//
//==========================================================

class Texture;
class Billboard;
class EnemyHige;

class EnemyHigeUI
{
public:
	EnemyHigeUI(EnemyHige* enemy);
	~EnemyHigeUI();
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	EnemyHige* hige_ = nullptr;			//�Ђ��̃|�C���^
	Billboard* lifeBar_ = nullptr;			//���C�t�o�[
};