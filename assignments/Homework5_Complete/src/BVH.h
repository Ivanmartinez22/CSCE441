#pragma once
#include "Shape.h"

#include <algorithm>    // std::swap
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Triangle.h"
#include <vector>
using std::vector;

class BVH : public Shape
{
public:
	BVH(void);
	bool intersection(Ray &ray);
	~BVH(void);

    glm::vec3 min;
    glm::vec3 max;
    int depth; // depth mod 3 == axis if 0 axis = x if 1 axis = y if 2 axis = z 
    vector<Shape*> shapes;
    BVH* left = nullptr;
    BVH* right = nullptr;
    vector<int> indices;
    glm::vec3 average_vertex;
    BVH** bvh_nodes;
    bool isLeaf = false;
    bool isRight = false;
    bool isLeft = false;
};
