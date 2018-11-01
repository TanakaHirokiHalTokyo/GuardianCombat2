#pragma once

//================================================================
//
//		Shotgun‚Ì’eƒNƒ‰ƒX
//
//================================================================

#include "Bullet.h"
class CEffekseer;

const unsigned int BulletNum = 6;		//U’e‚·‚é’e‚Ì”

class Bullet_Shotgun :public Bullet
{
public:
	Bullet_Shotgun();
	~Bullet_Shotgun();
	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
	
	void SetShooting();
private:
	CEffekseer* bullet_[BulletNum];		//U’e‚ÌƒGƒtƒFƒNƒg
	bool shot_ = false;					//”­Ë’†‚©
};