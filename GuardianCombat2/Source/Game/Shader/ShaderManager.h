#pragma once



class ShaderManager
{
public:
	struct FxFile
	{
		char fileName[256];
	};

	enum ShaderList
	{
		Shader_Toon,
		Shader_Outline,
		Shader_ShadowVolume,
		Shader_ShadowMap,
		Shader_VertexBlend,
		Shader_Max
	};
	static const FxFile shaderInfo_[Shader_Max];

	static void LoadAll();
	static void ReleaseAll();
	static LPD3DXEFFECT GetEffect(ShaderList type);

private:
	static LPD3DXEFFECT effect_[Shader_Max];
};