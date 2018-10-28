#include "PlayerCamera.h"
#include "Player.h"
#include "../../Vector3/Vector3.h"


PlayerCamera::PlayerCamera()
{
	//vector_ = new Vector3();
}

PlayerCamera::~PlayerCamera()
{
	
}

void PlayerCamera::Init()
{
	SetPosition(0,0,0);
	SetRotation(0,0,0);
	SetAt(0,0,0);
	vector_->SetFront(0.0f,0.0f,-1.0f);
	vector_->SetRight(-1.0f,0.0f,0.0f);
	vector_->SetUp(0.0f,1.0f,0.0f);

	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		//Player Vector Get
		const auto& player_vector = player_->GetVector();
		//ベクトルセット
		*vector_ = *player_vector;

		D3DXVECTOR3 camera_position = player_position + (player_vector->GetFront() * -1.0f) * armLength_;
		camera_position.y += 0.2f;

		SetPosition(camera_position);
		at_ = player_position;
		at_.y += 0.1f;
	}
	SetProjection();
}

void PlayerCamera::Uninit()
{
}

void PlayerCamera::Update()
{
	if (player_ != nullptr)
	{
		//PlayerPosition Get
		const auto& player_position = player_->GetPosition();
		//Player Vector Get
		const auto& player_vector = player_->GetVector();
		//ベクトルセット
		*vector_ = *player_vector;

		D3DXVECTOR3 camera_position = player_position + (player_vector->GetFront() * -1.0f) * armLength_;
		camera_position.y += 1.0f;

		SetPosition(camera_position);
		at_ = player_position;
		at_.y += 1.0f;
	}
	this->SetMainCameraInfo(this->transform_.pos, this->at_, vector_->GetUp(), this->mtxView_, this->mtxProj_);
	SetView();
}

void PlayerCamera::BeginDraw()
{
}

void PlayerCamera::Draw()
{
}

void PlayerCamera::EndDraw()
{
}

void PlayerCamera::SetPlayer(Player* player)
{
	player_ = player;
}
