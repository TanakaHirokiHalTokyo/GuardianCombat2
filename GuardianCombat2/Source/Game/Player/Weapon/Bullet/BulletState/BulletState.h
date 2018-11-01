#pragma once

//=======================================================================
//
//		弾の状態を制御
//
//=======================================================================

class Bullet;

class BulletState
{
public:
	virtual void Act(Bullet* bullet) = 0;
};