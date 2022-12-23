#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Plane :
	public Shape
{
public:
	Plane(void);
	bool intersection(Ray &ray, float t0, float t1, float &t);
	~Plane(void);
	glm::vec3 center;
	glm::vec3 normal;
	

};
