#include "Light.h"

#pragma once
class Scene
{
private:
	std::vector<DrawableObject*> drawObjects;
    std::vector<SceneLight*> lights;

public:
	Scene();
	~Scene();
	void AddObject(DrawableObject* obj);

	DrawableObject* GetObject(int index);

	void DrawAll();
};

