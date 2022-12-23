#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Ray.h"
struct TPair{
	float t0;
	float t1;
	bool intersection;
	int shape_type;//1 == sphere 2 == plane 3 == triangle
};

class Shape
{
public:
	Shape(void);
	// virtual bool intersection(Ray &ray, float &t);
	
	~Shape(void);


	glm::vec3 ka, kd, ks, km;
	float n;
	float t;
	int type; //sphere == 1 plane == 2 triangle == 3
	//common data 


	// //plane 
	// glm::vec3 center;
	// glm::vec3 normal;
	// //sphere 
	// glm::vec3 position;
	// float radius;
};

