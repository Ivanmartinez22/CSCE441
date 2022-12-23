#include "Scene.h"

Scene::~Scene()
{
}

Scene::Scene()
{
	
}

Scene::Scene(std::vector<Shape*> shapes, std::vector<Light*> lights)
{
	this->shapes = shapes;
    this->lights = lights;
}
