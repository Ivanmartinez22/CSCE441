#include "Triangle.h"
#include <iostream>
using std::cout;
using std::endl;

float alpha(float x, float y, float xa, float xb, float xc, float ya, float yb, float yc){
	float upper = (-1* (x - xb) * (yc -yb)) + ((y - yb) * (xc - xb));
	float lower = (-1 * (xa - xb) * (yc - yb)) + ((ya - yb) * (xc - xb));
	return upper/lower;
}

float beta(float x, float y, float xa, float xb, float xc, float ya, float yb, float yc){
	float upper = (-1 *(x -xc) * (ya - yc)) + ((y - yc) * (xa - xc));
	float lower = (-1*(xb - xc) * (ya - yc)) + ((yb - yc) * (xa - xc));
	return upper/lower;
}

float gamma(float alpha, float beta){
	
	return 1 - alpha - beta;
}

Triangle::Triangle(void)
{
    // this->center = glm::vec3(0, 0,0);
}



bool Triangle::intersection(Ray &ray, float t0, float t1, float &t){

    //optimized code runs a lot faster 
    glm::vec3 edge1 = vertex1 - vertex0;
    glm::vec3 edge2 = vertex2 - vertex0;
    
    glm::vec3 s1 = glm::cross(ray.direction, edge2);
    
    float S1_E1 = glm::dot(s1, edge1);

    if(S1_E1 > -0.0001 && S1_E1 < 0.0001){
        return false;
    }
    glm::vec3 s = ray.origin - vertex0;
    float factor = (1.0f/S1_E1);
    float b1 = factor * glm::dot(s1, s);
    if(b1 < 0 || b1 > 1){
        return false;
    }
    glm::vec3 s2 = glm::cross(s, edge1);
    float b2 = factor * glm::dot(s2,ray.direction);
    if(b2 < 0 || b2 > 1){
        return false;
    }

    t = factor * glm::dot(s2, edge2);


    float beta_var = b1;
    float gamma_var = b2;
    float alpha_var = 1 - beta_var - gamma_var;
    if((beta_var >= 0) && (beta_var <= 1) && (alpha_var >= 0) && (alpha_var  <= 1) && (gamma_var >= 0) && (gamma_var  <= 1)){
        calculated_normal = alpha_var*normal0 + beta_var*normal1 + gamma_var*normal2;
        return true;
    }
    return false;


}

Triangle::~Triangle(void)
{

}

