#ifndef _EFFECT_H_
#define _EFFECT_H_

#include <d3dx9.h>
#include <d3d9.h>
#include <string>

class EffectManager
{
public:
	struct VERTEX3D
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 vec;
		D3DCOLOR color;
		D3DXVECTOR2 texcoord;
	};

	static void Init();
	static void Uninit();

	static LPDIRECT3DTEXTURE9 GetTexture();									//�e�N�X�`���|�C���^�擾
	static LPDIRECT3DVERTEXBUFFER9& GetVertexBuffer();			//�o�[�e�b�N�X�o�b�t�@�擾
	static LPDIRECT3DINDEXBUFFER9& GetIndexBuffer();				//�C���f�b�N�X�o�b�t�@�擾
	static VERTEX3D& GetVertex();
private:
	static LPDIRECT3DTEXTURE9 texture_;									//�e�N�X�`���|�C���^
	static LPDIRECT3DVERTEXBUFFER9 vertexBuffer_;				//�o�[�e�b�N�X�o�b�t�@
	static LPDIRECT3DINDEXBUFFER9 indexBuffer_;					//�C���f�b�N�X�o�b�t�@
	static VERTEX3D* vertex_;
};

class AdditionEffect
{
public:
	static const int EFFECTNUM = 3000;

	struct Transform
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 scale;
	};

	AdditionEffect();
	~AdditionEffect();
	void Init();
	void Uninit();
	void Update();
	void Draw();

	inline void SetVisible(const bool flag) { visible_ = flag; }													//����Ԑݒ�
	inline bool GetVisible()const { return visible_; }																	//����Ԏ擾
	void CreateEffect(D3DXVECTOR3 pos);										//�G�t�F�N�g�쐬
	void SetLimit(int limit);																//�������Ԑݒ�
	inline int GetLimit() { return limit_; }											//�������Ԏ擾
	void SetColor(float r, float g, float b);											//�F�ݒ�
	void SetScale(float size);															//�T�C�Y�ݒ�
	inline float GetScale() { return transform_[0].scale.x; }				//�T�C�Y�擾
	inline D3DXCOLOR GetColor() { return color_[0]; }					//�J���[�擾
	void SaveParameter(std::string filename);										//�p�����[�^���t�@�C�������o��
	void LoadParameter(std::string filename);										//�p�����[�^���t�@�C���ǂݍ���
private:
	bool visible_ = false;						//�����
	int count_[EFFECTNUM];				//�J�E���^
	int limit_ = 60;									//����
	bool use_[EFFECTNUM];				//���ݎg�p����
	D3DXCOLOR color_[EFFECTNUM] = {};		//�F
	Transform transform_[EFFECTNUM] = {};		//�g�����X�t�H�[���f�[�^
};



#endif // !_EFFECT_H_
