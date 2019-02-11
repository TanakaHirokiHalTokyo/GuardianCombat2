#include "Scene.h"
#include "Fade.h"
#include "../Game/GameManager/GameManager.h"

bool Scene::ChangeSceneUpdate()
{
	Fade* fade = GameManager::GetFade();
	if (sceneChange_)
	{
		if (!fade->IsFading())
		{
			return true;
		}
	}
	return false;
}

void Scene::SceneChange(Scene* next_scene)
{
	Fade* fade = GameManager::GetFade();

	if (!sceneChange_)
	{
		sceneChange_ = true;
		fade->FadeIn();
	}
	else
	{
		if (!fade->IsFading())
		{
			GameManager::SetScene(next_scene);
			return;
		}
	}
}
