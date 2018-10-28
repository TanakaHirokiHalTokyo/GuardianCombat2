
#include "Light.h"
#include "../DirectXRenderer.h"
#include "../Imgui/ImguiManager.h"


//-------------------------------------------------------------------
//		DirectionalLight
//-------------------------------------------------------------------
DirectionalLight::DirectionalLight()
{
	ZeroMemory(&light_, sizeof(light_));
	light_.Type = D3DLIGHT_DIRECTIONAL;
	this->SetVector(D3DXVECTOR3(1.0f, -1.0f, 0.0f));

	light_.Diffuse.r = 1.0f;
	light_.Diffuse.g = 1.0f;
	light_.Diffuse.b = 1.0f;
	light_.Diffuse.a = 1.0f;

	light_.Ambient.r = 0.8f;
	light_.Ambient.g = 0.8f;
	light_.Ambient.b = 0.8f;
	light_.Ambient.a = 1.0f;

	CRendererDirectX::GetDevice()->LightEnable(0, TRUE);

	angle_ = 0.0f;
	vector_ = D3DXVECTOR3(1.0f,-1.0f,0.0f);
}
DirectionalLight::~DirectionalLight()
{

}
void DirectionalLight::Init()
{
	
}
void DirectionalLight::Uninit()
{

}
void DirectionalLight::Update()
{
	//D3DXMATRIX mtxRotate;
	//angle_ = 0.02f;
	//D3DXMatrixRotationY(&mtxRotate, D3DXToRadian(angle_));

	//D3DXVec3TransformNormal(
	//	&vector_,							//‰ñ“]‚³‚¹‚½‚¢ƒxƒNƒgƒ‹(Œü‚«‚×‚­‚Æ‚é)
	//	&vector_,
	//	&mtxRotate
	//);
	//this->SetVector(vector_);
}
void DirectionalLight::BeginDraw()
{
}
void DirectionalLight::Draw()
{

}
void DirectionalLight::EndDraw()
{
}
void DirectionalLight::SetVector(D3DXVECTOR3 vector)
{
	D3DXVec3Normalize((D3DXVECTOR3*)&light_.Direction, &vector);
	CRendererDirectX::GetDevice()->SetLight(0, &light_);
}

D3DXVECTOR3 DirectionalLight::GetVector()
{
	return vector_;
}

D3DXVECTOR4 DirectionalLight::GetAmbient()
{
	return D3DXVECTOR4(light_.Ambient.r,light_.Ambient.g,light_.Ambient.b,light_.Ambient.a);
}
