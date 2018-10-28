

#include "Effekseer.h"
#include "../DirectXRenderer.h"
#include "../Imgui/ImguiManager.h"
#include "../Camera/Camera.h"

static Camera* camera;
CEffekseer::CEffekseer(Effect EffectType)
{
	//�`��Ǘ��p�C���X�^���X�̐���
	effekseerRenderer_ = ::EffekseerRendererDX9::Renderer::Create(CRendererDirectX::GetDevice(), 2000);
	// �G�t�F�N�g�Ǘ��p�C���X�^���X�̐���
	effekseerManager_ = ::Effekseer::Manager::Create(2000);

	// �`��p�C���X�^���X����`��@�\��ݒ�
	effekseerManager_->SetSpriteRenderer(effekseerRenderer_->CreateSpriteRenderer());
	effekseerManager_->SetRibbonRenderer(effekseerRenderer_->CreateRibbonRenderer());
	effekseerManager_->SetRingRenderer(effekseerRenderer_->CreateRingRenderer());
	effekseerManager_->SetTrackRenderer(effekseerRenderer_->CreateTrackRenderer());
	effekseerManager_->SetModelRenderer(effekseerRenderer_->CreateModelRenderer());

	// �`��p�C���X�^���X����e�N�X�`���̓Ǎ��@�\��ݒ�
	// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
	effekseerManager_->SetTextureLoader(effekseerRenderer_->CreateTextureLoader());
	effekseerManager_->SetModelLoader(effekseerRenderer_->CreateModelLoader());

	//������W�n�ɐݒ�
	effekseerManager_->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);

	this->effectType_ = EffectType;

	this->SetProj(Fov, (float)ScreenWidth, (float)ScreenHeight, Proj_Near, Proj_Far);

	this->LoadEffect();

	this->SetPosition(0.0f,0.0f,0.0f);
	this->SetRotate(0.0f,0.0f,0.0f);
	this->SetScale(1.0f,1.0f,1.0f);
}

CEffekseer::~CEffekseer()
{
	// �G�t�F�N�g�̒�~
	this->Stop();

	//�G�t�F�N�g������B�Đ����̏ꍇ�͍Đ����I�������ہA�����I�ɉ��
	ES_SAFE_RELEASE(effekseerEffect_);

	//�G�t�F�N�g�Ǘ��p�C���X�^���X��j��
	effekseerManager_->Destroy();

	//�`��p�C���X�^���X��j��
	effekseerRenderer_->Destroy();

	effekseerHandle_ = NULL;

}

void CEffekseer::LoadEffect()
{
	//�G�t�F�N�g�̓ǂݍ���
	effekseerEffect_ = Effekseer::Effect::Create(effekseerManager_, (const EFK_CHAR*)fileName_[effectType_].fileName);
	if (effekseerEffect_ == NULL)
	{
		MessageBoxA(NULL, "�G�t�F�N�g�ǂݍ��ݎ��s", "ERROR", MB_OK);
	}

}
void CEffekseer::Init()
{
	
}
void CEffekseer::Uninit()
{
	
}
void CEffekseer::Update()
{
	CAMERA_INFO camInfo = Camera::GetCameraInfo();

	this->SetView(camInfo.pos, camInfo.at, camInfo.up);

	D3DXMATRIX mtxScale, mtxTrans, mtxWorld, mtxRotateX, mtxRotateY, mtxRotateZ;

	D3DXMatrixTranslation(&mtxTrans, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	D3DXMatrixScaling(&mtxScale, transform_.scale.x, transform_.scale.y, transform_.scale.z);
	D3DXMatrixRotationX(&mtxRotateX, D3DXToRadian(transform_.rotate.x));
	D3DXMatrixRotationY(&mtxRotateY, D3DXToRadian(transform_.rotate.y));
	D3DXMatrixRotationZ(&mtxRotateZ, D3DXToRadian(transform_.rotate.z));

	mtxWorld = mtxScale;
	mtxWorld *= mtxRotateX;
	mtxWorld *= mtxRotateY;
	mtxWorld *= mtxRotateZ;
	mtxWorld *= mtxTrans;

	this->SetMatrix(mtxWorld);
	SetSpeed(speed_);

	playing_ = effekseerManager_->Exists(effekseerHandle_);
	if (!playing_)
	{
		if (this->repeat_)
		{
			//�G�t�F�N�g�̍Đ�
			Play();
		}
		else
		{
			Stop();
		}
	}

	effekseerManager_->Flip();
	//���ׂẴG�t�F�N�g�̍X�V
	effekseerManager_->Update();
}
void CEffekseer::Draw()
{
	//effekseerRenderer_->SetRenderMode(Effekseer::RenderMode::Wireframe);

	if (GetVisible())
	{
		
		effekseerRenderer_->BeginRendering();
		effekseerManager_->Draw();
		effekseerRenderer_->EndRendering();
	}
}
void CEffekseer::SetProj(float Fov, float ScreenWidth, float ScreenHeight, float Near, float Far)
{
	//���e�s��̍X�V
	effekseerRenderer_->SetProjectionMatrix(Effekseer::Matrix44().PerspectiveFovLH(D3DXToRadian(Fov), ScreenWidth / ScreenHeight, Near, Far));
}
void CEffekseer::SetView(D3DXVECTOR3 Pos, D3DXVECTOR3 At, D3DXVECTOR3 Up)
{
	//�J�����s��̍X�V
	effekseerRenderer_->SetCameraMatrix(Effekseer::Matrix44().LookAtLH(
		Effekseer::Vector3D(Pos.x, Pos.y, Pos.z),
		Effekseer::Vector3D(At.x, At.y, At.z),
		Effekseer::Vector3D(Up.x, Up.y, Up.z)
	));
}
void CEffekseer::SetSpeed(float speed)
{
	speed_ = speed;
	effekseerManager_->SetSpeed(effekseerHandle_, speed_);
}
void CEffekseer::RepeatEffect(bool repeat)
{
	this->repeat_ = repeat;
}
void CEffekseer::Play()
{
	//�G�t�F�N�g�̍Đ�
	effekseerHandle_ = effekseerManager_->Play(effekseerEffect_, transform_.pos.x, transform_.pos.y, transform_.pos.z);

	effekseerManager_->Flip();

	
	playing_ = true;
}
void CEffekseer::Stop()
{
	//�G�t�F�N�g�̒�~
	effekseerManager_->StopEffect(effekseerHandle_);
}
void CEffekseer::SetMatrix(D3DXMATRIX mat)
{
	Effekseer::Matrix43 matrix;
	matrix.Value[0][0] = mat._11;
	matrix.Value[0][1] = mat._12;
	matrix.Value[0][2] = mat._13;
	matrix.Value[1][0] = mat._21;
	matrix.Value[1][1] = mat._22;
	matrix.Value[1][2] = mat._23;
	matrix.Value[2][0] = mat._31;
	matrix.Value[2][1] = mat._32;
	matrix.Value[2][2] = mat._33;
	matrix.Value[3][0] = mat._41;
	matrix.Value[3][1] = mat._42;
	matrix.Value[3][2] = mat._43;

	effekseerManager_->SetMatrix(effekseerHandle_,matrix);
}
bool CEffekseer::GetPlaying()
{
	return playing_;
}
void CEffekseer::SetVisible(bool flag)
{
	visible_ = flag;
}
bool CEffekseer::GetVisible()
{
	return visible_;
}
void CEffekseer::SetPosition(float x, float y, float z)
{
	transform_.pos = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetPosition(D3DXVECTOR3 trans)
{
	transform_.pos = trans;
}
D3DXVECTOR3 CEffekseer::GetPosition()
{
	return transform_.pos;
}
void CEffekseer::SetRotate(float x, float y, float z)
{
	transform_.rotate = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetRotate(D3DXVECTOR3 rot)
{
	transform_.rotate = rot;
}
D3DXVECTOR3 CEffekseer::GetRotate()
{
	return transform_.rotate;
}
void CEffekseer::SetScale(float x, float y, float z)
{
	transform_.scale = D3DXVECTOR3(x,y,z);
}
void CEffekseer::SetScale(D3DXVECTOR3 scale)
{
	transform_.scale = scale;
}
D3DXVECTOR3 CEffekseer::GetScale()
{
	return transform_.scale;
}
void CEffekseer::Paused(bool paused)
{
	this->paused_ = paused;
	effekseerManager_->SetPaused(effekseerHandle_, paused);
}
