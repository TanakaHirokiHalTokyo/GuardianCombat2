#include "WeaponRelative.h"
#include "../Weapon.h"
#include "../../Player.h"
#include "../../../../Camera/Camera.h"
#include "../../../../Imgui/ImguiManager.h"

void WeaponRelative::Act(Weapon * weapon)
{
	//����̐e�ɂȂ��Ă���v���C���[���擾
	const auto& player = weapon->GetPlayer();

	//�v���C���[�̃J�����̏����擾
	const auto& player_camera = player->GetCamera();

	//�v���C���[�̃��[���h���擾
	const auto& player_world = player->GetWorld();

	//�ʒu�ݒ�
	weapon->SetPosition(player_camera->GetPosition() + player_camera->GetFront() * 0.15f);
	weapon->SetPosition(weapon->GetPosition() + player_camera->GetRight() * 0.1f + player_camera->GetUp() * -0.05f);
	//��]�p�x�ݒ�
	weapon->SetRotation(player_camera->GetRotate().y,player_camera->GetRotate().x,0);

}
