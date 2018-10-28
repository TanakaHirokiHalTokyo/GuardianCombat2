
#include "Texture.h"
#include "../DirectXRenderer.h"

LPDIRECT3DTEXTURE9 TextureManager::pTexture_[TextureManager::Tex_Max];

//�e�N�X�`�����\����
const TEXTURE_3D TextureManager::texInfo_[TextureManager::TextureList::Tex_Max]
{
	"resource/texture/0.png",						29, 31,29,31,
	"resource/texture/Dot.png",						1, 1,1,1,
	"resource/texture/ToonPaint.png",				256, 256,256,256,
	"resource/texture/Mission.png",					395, 70,395,70,
};
void TextureManager::LoadAll()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�e�N�X�`���\�X�e�[�g�̐ݒ�
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	//WRAP...����
	//CLAMP...�����L�΂�
	//MIRROR...����
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�t�B���^�����O
	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	for (unsigned int i = 0; i < Tex_Max; i++)
	{
		hr = D3DXCreateTextureFromFileA(pDevice, texInfo_[i].fileName, &pTexture_[i]);
		if (FAILED(hr))
		{
			MessageBoxA(NULL, "�e�N�X�`���ǂݍ��ݎ��s", "�x��", MB_OK | MB_ICONHAND);
		}
	}
}
void TextureManager::ReleaseAll()
{
	for (unsigned int i = 0; i < Tex_Max; i++)
	{
		if (pTexture_[i] != nullptr)
		{
			pTexture_[i]->Release();
			pTexture_[i] = nullptr;
		}
	}
}
TextureManager::TexInfo TextureManager::GetTexture(TextureManager::TextureList type)
{
	TexInfo texture;

	texture.texcoordX = texInfo_[type].texcoordX;
	texture.texcoordY = texInfo_[type].texcoordY;
	texture.height = texInfo_[type].height;
	texture.width = texInfo_[type].width;
	texture.pTex = pTexture_[type];

	return texture;
}

Texture::Texture(TextureManager::TextureList type)
{
	Object::transform_.pos = D3DXVECTOR3(0,0,0);
	Object::transform_.scale = D3DXVECTOR3(1,1,1);

	TextureManager::TexInfo textureInfo = TextureManager::GetTexture(type);
	pTexture_ = textureInfo.pTex;
	this->texSize_ = D3DXVECTOR2((float)textureInfo.width, (float)textureInfo.height);
	this->texcoordSize_ = D3DXVECTOR2((float)textureInfo.texcoordX, (float)textureInfo.texcoordY);
	this->drawSize_ = D3DXVECTOR2((float)textureInfo.width, (float)textureInfo.height);
	this->texcoord_ = D3DXVECTOR2((float)0, (float)0);
	this->affine_ = false;
	this->color_ = D3DCOLOR_RGBA(255, 255, 255, 255);
}
Texture::~Texture()
{
	
}
void Texture::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	HRESULT hr;

	hr = pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4,	//���o�C�g�؂�邩 * �|���S����
		D3DUSAGE_WRITEONLY,		//�g�p�p�r �������݂̂݁@���g�͕ۏ؂���Ȃ��̂Ō��ɍs���Ȃ�
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,		//�f�o�C�X�ɊǗ�������
		&this->g_pVertexBuffer,		//�����݂̍菈
		NULL
	);
	if (FAILED(hr))
	{
		MessageBoxA(NULL, "���_�o�b�t�@������܂���ł����B", "ERROR", MB_OK);
	}
}
void Texture::Uninit()
{

	if (g_pVertexBuffer)
	{
		g_pVertexBuffer->Release();
		g_pVertexBuffer = NULL;
	}
}
void Texture::Update()
{

}
void Texture::BeginDraw()
{
}
void Texture::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRendererDirectX::GetDevice();
	if (pDevice == NULL) { return; }

	//�����_�\�X�e�[�g�̐ݒ�
	//���u�����h���s��
	//SRC...������`�����́B�܂�|���S���Ƀe�N�X�`�����`�悳�����́B
	//DEST...���łɕ`�悳��Ă����ʂ̂��ƁB
	//SRC_RGB * SRC_�� + DEST_RGB * (1 - SRC_��)
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	//�A���t�@�e�X�g��TRUE�ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 50);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	pDevice->SetFVF(FVF_VERTEX_2D);

	if (this->affine_ == true)
	{
		CreateVertexAffine(this->drawSize_.x, this->drawSize_.y, (int)this->texcoord_.x, (int)this->texcoord_.y, (int)this->texcoordSize_.x, (int)this->texcoordSize_.y);
	}
	else
	{
		CreateVertex(this->drawSize_.x, this->drawSize_.y, (int)this->texcoord_.x, (int)this->texcoord_.y, (int)this->texcoordSize_.x, (int)this->texcoordSize_.y, (int)this->texSize_.x, (int)this->texSize_.y);
	}
	//GPU��VertexBuffer���p�C�v���C���łȂ�-+
	pDevice->SetStreamSource(
		0,
		g_pVertexBuffer,
		0,
		sizeof(VERTEX_2D)//�ׂ̒��_�܂łǂꂭ�炢�̒���
	);
	pDevice->SetTexture(0, this->pTexture_);
	//�|���S����`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,						//���o�C�g�ڂ���ǂݍ��ނ�
		2						//�O�p�`
	);
	//�A���t�@�e�X�g��FALSE�ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}
void Texture::EndDraw()
{
}
void Texture::CreateVertex(float dw, float dh, int tcx, int tcy, int tcw, int tch, int texW, int texH)
{
	VERTEX_2D* pV;

	g_pVertexBuffer->Lock(
		0,				//�������烍�b�N���Ăق���
		0,				//�����܂Ń��b�N 0�������ƑS���؂�邱�Ƃ��ł���
		(void**)&pV,	//�^���A�h���X�@�b�o�t�̃������ɂ�������X�g�A���ăA�����b�N�����Ƃ��ɂu�q�`�l�ɓ]�������
		D3DLOCK_DISCARD	//�ӂ炮�@�O�ł������������׸ނ������ƌ������悭�Ȃ�
	);

	int tw = texW;
	int th = texH;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

	pV[0].pos = D3DXVECTOR4(transform_.pos.x + 0.0f - 0.5f, transform_.pos.y + 0.0f - 0.5f, 0.0f, 0.1f);
	pV[1].pos = D3DXVECTOR4(transform_.pos.x + dw - 0.5f, transform_.pos.y + 0.0f - 0.5f, 0.0f, 0.1f);
	pV[2].pos = D3DXVECTOR4(transform_.pos.x + 0.0f - 0.5f, transform_.pos.y + dh - 0.5f, 0.0f, 0.1f);
	pV[3].pos = D3DXVECTOR4(transform_.pos.x + dw - 0.5f, transform_.pos.y + dh - 0.5f, 0.0f, 0.1f);

	pV[0].color =
	pV[1].color =
	pV[2].color =
	pV[3].color = this->color_;

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	g_pVertexBuffer->Unlock();

}
void Texture::CreateVertexAffine(float dw, float dh, int tcx,int tcy, int tcw, int tch)
{
	VERTEX_2D* pV;
	g_pVertexBuffer->Lock(0, 0, (void**)&pV, D3DLOCK_DISCARD);



	float x = (-dw - scaleOffsetX_) * scaleOffsetX_ + scaleOffsetX_ + dw;
	float x_w = (dw - scaleOffsetX_) * scaleOffsetX_ + scaleOffsetX_;
															 
	float y = (-dh - scaleOffsetY_) * scaleOffsetY_ + scaleOffsetY_ + dh;
	float y_h = (dh - scaleOffsetY_) * scaleOffsetY_ + scaleOffsetY_;

	float fRotationOffsetX = rotationOffsetX_;
	float fRotationOffsetY = rotationOffsetY_;

											
	x -= fRotationOffsetX;
	x_w -= fRotationOffsetX;

	y -= fRotationOffsetY;
	y_h -= fRotationOffsetY;

	transform_.pos.x += fRotationOffsetX - 0.5f;
	transform_.pos.y += fRotationOffsetY - 0.5f;

	int tw = (int)texSize_.x;
	int th = (int)texSize_.y;

	float u0 = (float)tcx / tw;
	float v0 = (float)tcy / th;
	float u1 = (float)(tcx + tcw) / tw;
	float v1 = (float)(tcy + tch) / th;

									
	pV[0].pos = D3DXVECTOR4(transform_.pos.x + x  * cosf(angle_) - y  * sinf(angle_), transform_.pos.y + x  * sinf(angle_) + y  * cosf(angle_), 0.0f, 0.1f);
	pV[1].pos = D3DXVECTOR4(transform_.pos.x + x_w * cosf(angle_) - y  * sinf(angle_), transform_.pos.y + x_w * sinf(angle_) + y  * cosf(angle_), 0.0f, 0.1f);
	pV[2].pos = D3DXVECTOR4(transform_.pos.x + x  * cosf(angle_) - y_h * sinf(angle_), transform_.pos.y + x  * sinf(angle_) + y_h * cosf(angle_), 0.0f, 0.1f);
	pV[3].pos = D3DXVECTOR4(transform_.pos.x + x_w * cosf(angle_) - y_h * sinf(angle_), transform_.pos.y + x_w * sinf(angle_) + y_h * cosf(angle_), 0.0f, 0.1f);

	pV[0].texcoord = D3DXVECTOR2(u0, v0);
	pV[1].texcoord = D3DXVECTOR2(u1, v0);
	pV[2].texcoord = D3DXVECTOR2(u0, v1);
	pV[3].texcoord = D3DXVECTOR2(u1, v1);

	pV[0].color =
		pV[1].color =
		pV[2].color =
		pV[3].color = color_;

	g_pVertexBuffer->Unlock();
}
//-------------------------------------------------------------
//	Rotation
//		cx cy : ��]������ۂǂ�����ɉ�]���邩�B
//				�^�񒆂ŉ�]���������ꍇ�A���̉摜�T�C�Y��1/2
//-------------------------------------------------------------
void Texture::SetRotation(float cx, float cy, float angle)
{
	this->angle_ = angle;
	this->rotationOffsetX_ = cx;
	this->rotationOffsetY_ = cy;
}
//------------------------------------------------------------
//	Scale
//		cx cy:�ǂ�����g�傳���邩�B
//		scaleX Y : �g�嗦�B���̉摜�T�C�Y�ɑ΂���g�嗦�B
//------------------------------------------------------------
void Texture::SetScale(float cx, float cy, float scaleX, float scaleY)
{
	this->scaleOffsetX_ = cx;
	this->scaleOffsetY_ = cy;
	Object::transform_.scale.x = scaleX;
	Object::transform_.scale.y = scaleY;
}
void Texture::SetColor(D3DCOLOR color)
{
	this->color_ = color;
}
D3DCOLOR Texture::GetColor()
{
	return color_;
}
TEXPATTERN Texture::ReturnTexPattern(int Count, int Frame, int texWNum, int texHNum, int texW, int texH)
{
	TEXPATTERN texpattern;
	texpattern.patternNum = (Count / Frame) % (texWNum * texHNum);
	texpattern.patternW = texpattern.patternNum % texWNum;
	texpattern.patternH = texpattern.patternNum / texWNum;
	texpattern.tcx = texpattern.patternW * texW;
	texpattern.tcy = texpattern.patternH * texH;

	return texpattern;
}
void Texture::SetDrawSize(float width, float height) {
	this->drawSize_.x = width;
	this->drawSize_.y = height;
}
void Texture::SetTexcoord(int tcx, int tcy) {
	this->texcoord_.x = (FLOAT)tcx;
	this->texcoord_.y = (FLOAT)tcy;
}
void Texture::SetTexture(LPDIRECT3DTEXTURE9 pTex, float texW, float texH)
{
	this->pTexture_ = pTex;
	this->texSize_.x = texW;
	this->texSize_.y = texH;
}
void Texture::SetTexcoordSize(float tcw, float tch) {
	this->texcoordSize_.x = tcw;
	this->texcoordSize_.y = tch;
}
void Texture::SetAffine(bool Affine) {
	this->affine_ = Affine;
}


