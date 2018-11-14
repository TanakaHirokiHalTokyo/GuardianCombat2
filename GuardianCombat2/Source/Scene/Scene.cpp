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
