#pragma once
#include <vector>
#include "Scene.h"
#include <cmath>
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include <glm/glm.hpp>

using std::vector;

//testing 
class Camera
{
public:
	Camera();
	~Camera();

	Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 look_at, glm::vec3 up, int FovY, int focal_distance);
	Ray CameraRay(int x, int y);
	glm::vec3 RayTime(Ray ray,  int time);
	glm::vec3 ComputeRayColor(Ray ray, Shape * hit_shape, Scene* scene, int depth,float t0, float t1);
	void TakePicture(Scene *scene);
	float* GetRenderedImage() { return renderedImage; };
	Shape* Hit(Ray ray, float t0, float t1, vector<Shape*> shapes);

private:

	int widthRes;
	int heightRes;

	glm::vec3 eye;
	glm::vec3 look_at;
	glm::vec3 up;
	glm::vec3 w;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 origin;

	float ly;
	float lx;

	float pixel_width;

	int FovY;
	int focal_distance;
	
	float *renderedImage;


};

