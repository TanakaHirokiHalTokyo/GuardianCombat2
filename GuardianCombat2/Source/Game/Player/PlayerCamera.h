#pragma once

#include "../../Camera/Camera.h"
class Player;

constexpr float ARMLENGTH = 1.5f;

class PlayerCamera :public Camera
{
public:
	PlayerCamera();
	~PlayerCamera();
	void Init();
	void Uninit();
	void Update();
	void BeginDraw();
	void Draw();
	void EndDraw();

	void SetPlayer(Player* player);

private:
	Player* player_ = nullptr;		//�v���C���[�̃|�C���^
	float armLength_ = ARMLENGTH;	//�v���C���[�ƃJ�����̋���
};