#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Sphere : public Shape
{
public:
	Sphere(void);
	bool intersection(Ray &ray, float t0, float t1, float &t);
	~Sphere(void);

	//sphere 
	glm::vec3 position;
	float radius;

};
