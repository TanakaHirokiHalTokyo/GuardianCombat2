#pragma once

#include "../../Scene/Scene.h"

class Fade;
class Scene;
class DirectionalLight;

class GameManager
{
public:
	enum SCENES
	{
		SCENE_GAME,
		SCENE_MAX,
	};
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static void SetScene(Scene* scene);
	static Scene* GetScene();
	static Fade* GetFade();
	static void SetDirectionalLight(DirectionalLight* light);
	static DirectionalLight* GetDirectionalLight();
private:
	static Scene* scene_;
	static Fade* fade_;
	static DirectionalLight* light_;
};