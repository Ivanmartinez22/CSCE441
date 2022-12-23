#pragma once
#include <vector>
#include "Shape.h"
#include "Light.h"


class Scene
{
public:
	Scene();
	Scene(std::vector<Shape*> shapes, std::vector<Light*> lights);
	~Scene();

// private:
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;

};
