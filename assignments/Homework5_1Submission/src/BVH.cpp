#include "BVH.h"
#include <iostream>
using std::cout;
using std::endl;
BVH::BVH(void){
    // this->position = glm::vec3(0, 0,0);
}

float min(float a, float b){
	float return_value = a;
	if(a < b){
		return_value = a;
	}
	else{
		return_value = b;
	}
	return return_value;
}

bool BVH::intersection(Ray &ray, float t0, float t1, float &t){

}


BVH::~BVH(void)
{
}

