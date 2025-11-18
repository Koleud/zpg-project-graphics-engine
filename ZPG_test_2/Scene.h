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
    int GetObjectCount() const;

	DrawableObject* GetObject(int index);
    std::vector<DrawableObject*> GetAllObjects();

    void RemoveObject(int index);

	void DrawAll();
};

