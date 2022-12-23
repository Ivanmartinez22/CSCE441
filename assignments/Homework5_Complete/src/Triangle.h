#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Triangle :
	public Shape
{
public:
	Triangle(void);
	bool intersection(Ray &ray, float t0, float t1, float &t);
	~Triangle(void);
	glm::vec3 vertex0;
	glm::vec3 vertex1;
	glm::vec3 vertex2;
	glm::vec3 normal0;
	glm::vec3 normal1;
	glm::vec3 normal2;
	glm::vec3 calculated_normal;
	glm::vec3 average_vertex;
	int index;
};
