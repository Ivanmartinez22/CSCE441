#include "BVH.h"
#include <iostream>
using std::cout;
using std::endl;
BVH::BVH(void){
    // this->position = glm::vec3(0, 0,0);
}

// float min(float a, float b){
// 	float return_value = a;
// 	if(a < b){
// 		return_value = a;
// 	}
// 	else{
// 		return_value = b;
// 	}
// 	return return_value;
// }

// float max(float a, float b){
// 	float return_value = a;
// 	if(a > b){
// 		return_value = a;
// 	}
// 	else{
// 		return_value = b;
// 	}
// 	return return_value;
// }

bool BVH::intersection(Ray &ray){
	
    
    float txmin = (min.x - ray.origin.x) / ray.direction.x; 
    float txmax = (max.x - ray.origin.x) / ray.direction.x; 
    
    float tmin = fmin(txmin, txmax);
    float tmax = fmax(txmin, txmax);
 
    float tymin = (min.y - ray.origin.y) / ray.direction.y; 
    float tymax = (max.y - ray.origin.y) / ray.direction.y;

    if(tymin > tymax){
        float temp = tymin;
        tymin = tymax;
        tymax = temp;
    }

    if(tmin > tymax){
        return false; 
    }
    if(tymin > tmax){
        return false;
    }
    if (tymin > tmin){
        tmin = tymin; 
    } 
    if (tymax < tmax){
        tmax = tymax; 
    } 
       
 
    float tzmin = (min.z - ray.origin.z) / ray.direction.z; 
    float tzmax = (max.z - ray.origin.z) / ray.direction.z; 
 
    if (tzmin > tzmax){
        float temp = tzmin;
        tzmin = tzmax;
        tzmax = temp;
    }

    if(tmin > tzmax){
        return false;
    }
    if(tzmin > tmax){ 
        return false; 
    }
    return true;

}


BVH::~BVH(void)
{
}

