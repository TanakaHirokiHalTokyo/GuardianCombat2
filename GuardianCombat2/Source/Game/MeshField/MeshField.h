#pragma once

#include	<d3d9.h>
#include	<d3dx9.h>
#include	"../Object.h"

const float FIELD_SIZE = 11.0f;

class XModel;

typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 vBuf;		//�o�[�e�b�N�X�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	iBuf;		//�C���f�b�N�X�o�b�t�@
	int	vNum;							//�o�[�e�b�N�X��
	int iNum;							//�C���f�b�N�X��
}MeshF;

class MeshField : public Object
{
public:
	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	enum Texture_List										//TextureList 
	{
		Tex_Turret,
		Tex_Difficulity,
		Tex_Max,
	};
	enum MeshField_Type							//�t�B�[���h���
	{
		Field_Ground,										//�n��
		Field_Wall,											//��
		Field_Max
	};
	enum Wall_Type										
	{
		Wall_Left,					//����
		Wall_Front,					//��O��
		Wall_Right,					//�E��
		Wall_Back,					//����
		Wall_Up						//���
	};

public:
	MeshField();		
	~MeshField();

	void Init()override;
	void Uninit()override;
	void Update()override;
	void BeginDraw()override;
	void Draw()override;
	void Draw(LPD3DXEFFECT effect, UINT pass);
	void EndDraw()override;

	float GetSizeX();				//���T�C�Y�擾
	float GetSizeY();				//���s�T�C�Y�擾
	float GetSpliteX();			//���������擾
	float GetSpliteY();			//���s�������擾

	static float GetHeight(D3DXVECTOR3 position);			//�w�肵�����W�̃t�B�[���h�̍����擾

private:
	void SetMeshField(float SizeX, float SizeY, int SpliteX, int SpliteY, MeshField_Type type);
private:
	LPDIRECT3DTEXTURE9 pTexture_ = nullptr;			//�e�N�X�`���|�C���^

	static float sizeX_;								//���T�C�Y
	static float sizeY_;								//���s�T�C�Y
	static int spliteX_;								//��������
	static int spliteY_;								//���s������
	static VERTEX3D*	Vertex;					//���_���
	static WORD*		pIndex;						//�C���f�b�N�X���

	D3DMATERIAL9 mat_;					//�}�e���A�����
	MeshF meshf_[MeshField::MeshField_Type::Field_Max];												//���[���h�s��
	MeshField_Type type_;					//���b�V���t�B�[���h�̎��
	D3DXMATRIX world_;						//���[���h�s��

	XModel* wall_[4] = {};											//�ǂ̃��f��
};