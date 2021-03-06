#pragma once


class Scene
{
public:
	virtual ~Scene() {}
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void BeginDraw() = 0;
	virtual void Draw() = 0;
	virtual void EndDraw() = 0;

	bool ChangeSceneUpdate();
	void SceneChange(Scene* next_scene);
protected:
	bool sceneChange_ = false;
	bool cursorActive_ = true;
};

