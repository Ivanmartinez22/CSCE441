#include "Plane.h"



Plane::Plane(void)
{
    // this->center = glm::vec3(0, 0,0);
}


bool Plane::intersection(Ray &ray, float t0, float t1, float &t){
    float t_ = glm::dot((center - ray.origin), normal);
    t_ /= glm::dot(ray.direction,  normal);

    if(t_ >= t0 && t_ < t1){
        t = t_;
        return true;
    }
    return false;

}

Plane::~Plane(void)
{

}

