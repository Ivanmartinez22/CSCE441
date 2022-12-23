#pragma once
#include "Shape.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class BVH : public Shape
{
public:
	BVH(void);
	bool intersection(Ray &ray, float t0, float t1, float &t);
	~BVH(void);
    float xmin = 400;
    float xmax = 800;
    float ymin = 200;
    float ymax = 600;
    float zmin = 100;
    float zmax = 200;

};
