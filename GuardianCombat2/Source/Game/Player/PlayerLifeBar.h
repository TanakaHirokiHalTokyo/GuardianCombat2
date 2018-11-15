#pragma once
//=========================================================
//
//			Player ���C�t�o�[
//
//=========================================================
#include "../Object.h"
class Texture;
class Player;

class PlayerLifeBar :public Object
{
public:
	PlayerLifeBar(Player* player);
	~PlayerLifeBar();
	void Init()override;	
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void EndDraw()override;
private:
	Texture* texture_ = nullptr;					//�e�N�X�`���|�C���^
	Player* player_ = nullptr;					//�v���C���[�|�C���^
};