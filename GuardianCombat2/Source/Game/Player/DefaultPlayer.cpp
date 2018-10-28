#include "DefaultPlayer.h"
#include "../SkinMesh/sceneSkinMesh.h"
#include "../../Vector3/Vector3.h"
#include "../SkinMesh/SkinMeshFile.h"
#include "PlayerCamera.h"
#include "AnimationState\StatePlayer_Idle.h"
#include "../../DInput/DirectInput.h"
#include "../MeshField/MeshField.h"
#include "../../Imgui/ImguiManager.h"

#define PLAYER_ROT_KEY1 (DIK_Q)
#define PLAYER_ROT_KEY2 (DIK_E)
constexpr float ROTATE_CAMERA_SPEED = 1.0f;

DefaultPlayer::DefaultPlayer()
{
	//SkinMesh Create
	animationModel_ = Object::Create<SceneSkinMesh>(SKINMESH_SOURCE[DEFAULT_PLAYER]);

	//Vector Create
	vector_ = new Vector3();

	//CameraCreate
	playerCamera_ = new PlayerCamera();
	playerCamera_->SetPlayer(this);
	playerCamera_->Init();

	//Set Use Camera
	usingCamera_ = playerCamera_;

	//animation state Create
	playerState_ = new StatePlayer_Idle();
}

DefaultPlayer::~DefaultPlayer()
{
	if (vector_ != nullptr)
	{
		delete vector_;
		vector_ = nullptr;
	}
	if (playerCamera_ != nullptr)
	{
		playerCamera_->Uninit();
		delete playerCamera_;
	}
	if (playerState_ != nullptr)
	{
		delete playerState_;
		playerState_ = nullptr;
	}
	usingCamera_ = nullptr;
}

void DefaultPlayer::Init()
{
	if (animationModel_ != nullptr)
	{
		animationModel_->SetScale(0.5f,0.5f,0.5f);
		animationModel_->SetPosition(0,0,0);
		animationModel_->SetRotation(0,0,0);
		animationModel_->ChangeAnim(IDLE,0.0f);
	}
	if (vector_ != nullptr)
	{
		vector_->SetFront(0.0f,0.0f, -1.0f);
		vector_->SetRight(-1.0f,0.0f,0.0f);
		vector_->SetUp(0.0f,1.0f,0.0f);
	}
	if (playerCamera_ != nullptr)
	{
		playerCamera_->Init();
	}
}

void DefaultPlayer::Uninit()
{
	if (playerCamera_ != nullptr)
	{
		playerCamera_->Uninit();
	}
}

void DefaultPlayer::Update()
{
	RotatePlayer();

	if (playerState_ != nullptr)
	{
		//AnimationState更新
		playerState_->UpdateKeyboard(this);
	}

	if (usingCamera_ != nullptr)
	{
		//使用しているカメラの更新処理
		usingCamera_->Update();
	}
	float y = MeshField::GetHeight(GetPosition());
	if (onLand_)
	{
		transform_.pos.y = y;
		ImGui::Begin("FieldHeight");
		ImGui::Text("%f",y);
		ImGui::End();
	}
	else
	{
		if (transform_.pos.y <= y)
		{
			onLand_ = true;
			transform_.pos.y = y;
		}
	}

	//アニメーションモデル更新
	animationModel_->SetPosition(GetPosition());
	animationModel_->SetRotation(GetRotate());
}

void DefaultPlayer::BeginDraw()
{
	D3DXMATRIX trans, rotX, rotY, rotZ, scale;
	D3DXMatrixTranslation(&trans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixRotationX(&rotX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationX(&rotY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationX(&rotZ, D3DXToRadian(transform_.rotate.z));
	D3DXMatrixScaling(&scale, transform_.scale.x, transform_.scale.y, transform_.scale.z);

	D3DXMatrixIdentity(&world_);

	world_ = scale;
	world_ *= rotX;
	world_ *= rotY;
	world_ *= rotZ;
	world_ *= trans;
}

void DefaultPlayer::Draw()
{
	
}

void DefaultPlayer::EndDraw()
{
}

SceneSkinMesh * DefaultPlayer::GetAnimModel()
{
	return animationModel_;
}

void DefaultPlayer::ChangeState(StatePlayer* state)
{
	if (playerState_ != nullptr)
	{
		delete playerState_;
	}
	playerState_ = state;
}

void DefaultPlayer::RotatePlayer()
{
	if (GetKeyboardPress(PLAYER_ROT_KEY1))
	{
		D3DXMATRIX mtxRotateY;
		transform_.rotate.y -= ROTATE_CAMERA_SPEED;
		D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(-ROTATE_CAMERA_SPEED));

		D3DXVECTOR3 front, right;

		D3DXVec3TransformNormal(&front, &vector_->GetFront(), &mtxRotateY);
		D3DXVec3Normalize(&front, &front);
		vector_->SetFront(front);
		D3DXVec3Cross(&right, &vector_->GetFront(), &vector_->GetUp());
		D3DXVec3Normalize(&right, &right);
		vector_->SetRight(right);
	}
	if (GetKeyboardPress(PLAYER_ROT_KEY2))
	{
		D3DXMATRIX mtxRotateY;
		transform_.rotate.y += ROTATE_CAMERA_SPEED;
		D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(ROTATE_CAMERA_SPEED));

		D3DXVECTOR3 front, right;

		D3DXVec3TransformNormal(&front, &vector_->GetFront(), &mtxRotateY);
		D3DXVec3Normalize(&front, &front);
		vector_->SetFront(front);
		D3DXVec3Cross(&right, &vector_->GetFront(), &vector_->GetUp());
		D3DXVec3Normalize(&right, &right);
		vector_->SetRight(right);
	}
}
