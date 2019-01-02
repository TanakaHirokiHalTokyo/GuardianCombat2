#pragma once

//================================================================
//
//		Shotgun‚Ì’eƒNƒ‰ƒX
//
//================================================================

#include "Bullet.h"

const size_t BulletAliveCount = 180;

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
	                  
private:
	
};