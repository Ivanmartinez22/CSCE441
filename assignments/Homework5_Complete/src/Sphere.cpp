#include "Sphere.h"
#include <iostream>
using std::cout;
using std::endl;
Sphere::Sphere(void){
    // this->position = glm::vec3(0, 0,0);
}

float min_(float a, float b){
	float return_value = a;
	if(a < b){
		return_value = a;
	}
	else{
		return_value = b;
	}
	return return_value;
}

bool Sphere::intersection(Ray &ray, float t0, float t1, float &t){
    glm::vec3 ray_origin = ray.origin;
    glm::vec3 ray_direction = ray.direction;

    float a = glm::dot(ray_direction, ray_direction);

    glm::vec3 view = ray_origin - position;

    //a == 1 
    float b = 2.0f * glm::dot((view), ray_direction);
    float c = glm::dot(view, view) - (radius*radius);
    float det = (b*b)-(4.0*a*c);
    if(det < 0){
        //t = -1;
        return false;
    }
    det = sqrt(det);
    // cout << a << " a " << b  << " b " << c << " c " << det << " d" << endl; 
    // if(det < 0){
    //     return false;
    // }
    float t0_ = (-b + det)/(2.0);
        float t1_ = (-b - det)/(2.0);
        t = min_(t0_, t1_);
    
    if(t >= t0 && t < t1){// one root t0 == t1
        // t = -0.5 * (b/a);
        
        return true;
    }
    else{ // no roots no interection
        //t = -1;
        return false;
    }

}

Sphere::~Sphere(void)
{
}

