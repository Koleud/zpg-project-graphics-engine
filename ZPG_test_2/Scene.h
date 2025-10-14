#pragma once
class Scene
{
private:
	std::vector<DrawableObject*> drawObjects;

public:
	Scene();
	~Scene();
	void AddObject(DrawableObject* obj);
	DrawableObject* GetObject(int index);
	void DrawAll();
};

