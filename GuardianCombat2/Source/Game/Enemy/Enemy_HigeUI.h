#pragma once

//==========================================================
//
//		HigeのUI管理
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
	EnemyHige* hige_ = nullptr;			//ひげのポインタ
	Billboard* lifeBar_ = nullptr;			//ライフバー
};