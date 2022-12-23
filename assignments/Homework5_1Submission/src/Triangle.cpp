#include "Triangle.h"
#include <iostream>
using std::cout;
using std::endl;

Triangle::Triangle(void)
{
    // this->center = glm::vec3(0, 0,0);
}



bool Triangle::intersection(Ray &ray, float t0, float t1, float &t){
    cout << "calling intersect" << endl;
    glm::vec3 edge1 = vertex1 - vertex0;
    glm::vec3 edge2 = vertex2 - vertex0;
    glm::vec3 h = glm::cross(ray.direction, edge2);
    float a = glm::dot(edge1, h);
    if(a > -0.0001 && a < 0.0001){
        cout << "a" << endl;
        return false;
    }
    float f = 1/a;
    glm::vec3 s = ray.origin - vertex0;
    float u = f * glm::dot(s, h);
    if(u < 0 || u > 1){
        cout << "u" << endl;
        return false;
    }
    glm::vec3 q =  glm::cross(s, edge1);
    float v = f * glm::dot(ray.direction, q);
    if (v < 0 || u + v > 1){cout << "v" << endl; return false;}
    const float t_ = f * dot( edge2, q );
    if (t_ > t0) t = t_;
    cout << "complete" << endl;
    return true;

}

Triangle::~Triangle(void)
{

}

