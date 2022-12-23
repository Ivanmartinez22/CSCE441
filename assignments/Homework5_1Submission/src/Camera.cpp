#include "Camera.h"
#include <cstring>
#include <iostream>
using std::cout;
using std::endl;
float max(float a, float b){
	float return_value = 0.0;
	if(a > b){
		return_value = a;
	}
	else{
		return_value = b;
	}
	return return_value;
}


//testing
Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera::Camera(int widthRes, int heightRes, glm::vec3 eye, glm::vec3 look_at, glm::vec3 up, int FovY, int focal_distance)
{
	this->widthRes = widthRes;
	this->heightRes = heightRes;
	this->eye = eye;
	this->look_at = look_at;
	this->up = up;
	this->FovY = FovY;
	this->focal_distance = focal_distance;
	float aspectRatio = (float)widthRes/(float)heightRes;
	this->w = glm::normalize(eye - look_at);
	this->u = glm::normalize(glm::cross(up, w));
	this->v = glm::normalize(glm::cross(w, u));

	this->ly = 2 * focal_distance * tan(FovY/2.0f);
	this->lx = ly * aspectRatio;
	this->pixel_width = ly/heightRes;
	glm::vec3 view_direction = glm::normalize(look_at - eye);
	glm::vec3 Ic = eye + view_direction * (float)focal_distance;
	this->origin = Ic - ((lx/2.0f)*u) - ((ly/2.0f)*v);


	renderedImage = new float[widthRes * heightRes * 3];
}

Ray Camera::CameraRay(int x, int y){
	glm::vec3 pixel_center = origin + (x + 0.5f) * pixel_width * u + (y + 0.5f) * pixel_width * v; 
	glm::vec3 direction = glm::normalize(pixel_center - eye);
	Ray resulting_primary_ray = Ray(eye, direction);
	return resulting_primary_ray;
}

Shape* Camera::Hit(Ray ray, float t0, float t1, vector<Shape*> shapes){// could prove buggy but good start
	float t = t1;
	float t_iter = 0.0;
	Shape * min = nullptr;
	//cout << "called hit" << endl;
	for(int i = 0; i < shapes.size(); i++){
		if(shapes[i]->type == 1){
			//cout << "entering sphere" << endl;
			Sphere* sphere = static_cast<Sphere*>(shapes[i]);
			// cout << t << endl;
			if(sphere->intersection(ray,t0, t1, t_iter)){
				//cout << "intersecting sphere" << endl;
				if(t_iter < t){
					min = shapes[i];
					t = t_iter;
					min->t = t;

					// cout << t << endl;
				}
			}
			// cout << t << endl;

		}
		else if(shapes[i]->type == 2){
			
			Plane* plane = static_cast<Plane*>(shapes[i]); 
			if(plane->intersection(ray, t0, t1, t_iter)){
				//cout << "intersecting plane" << endl;
				if(t_iter < t && (t_iter>= t0)){
					min = shapes[i];
					t = t_iter;
					min->t = t;
				}
			}
		}
		else if(shapes[i]->type == 3){
			
			Triangle* triangle = static_cast<Triangle*>(shapes[i]); 
			if(triangle->intersection(ray, t0, t1, t_iter)){
				//cout << "intersecting triangle" << endl;
				if(t_iter < t && (t_iter>= t0)){
					min = shapes[i];
					t = t_iter;
					min->t = t;
				}
			}
		}
		

	}
	if(min != nullptr){
		//cout << min->t << endl;
	}

	return min;

}

glm::vec3 Camera::RayTime(Ray ray, int time){
    return ray.origin + (ray.direction * (float)time);
}

glm::vec3 Camera::ComputeRayColor(Ray ray, Shape * hit_shape, Scene* scene, int depth, float t0, float t1){

			
	float epsilon = 0.0001;

	float t = hit_shape->t;

	glm::vec3 color = glm::vec3(0);
	glm::vec3 normal;
	glm::vec3 surface_point = ray.origin + ray.direction*t;
	if(hit_shape->type == 1){
		color = hit_shape->ka;
		
		Sphere* sphere = static_cast<Sphere*>(hit_shape); 
		normal = glm::normalize(surface_point - sphere->position);
		//normal = ((ray, t) - sphere->position);
	}
	else if(hit_shape->type == 2){
		color = hit_shape->ka;
		Plane* plane = static_cast<Plane*>(hit_shape); 
		normal = glm::normalize(plane->normal);
		// normal = (plane->normal);
	}
	else if(hit_shape->type == 3){
		color = hit_shape->ka;
		Triangle* triangle = static_cast<Triangle*>(hit_shape); 
		normal = glm::normalize(triangle->normal);
		// normal = (plane->normal);
	}

		

	// if((depth >= 2)){
	// 	return 	color;
	// }

	// glm::vec3 reflection = 2 * glm::dot(ray.direction, normal) * normal - ray.direction;
	
		


	for(int i = 0; i < scene->lights.size();i++){ //some calculations may be wrong
		glm::vec3 light = normalize(scene->lights[i]->position - surface_point);
		
		glm::vec3 eye_ = glm::normalize(ray.origin - surface_point);
		
			glm::vec3 reflection = ((2*(glm::dot(light, normal)))*normal) - light;
		// glm::vec3 reflection = glm::normalize(reflect(light, normal));
		float L_N = glm::dot(light, normal);
		float R_E = glm::dot(reflection, eye_);

		glm::vec3 diffuse_lighting = hit_shape->kd * (float)fmax(0, L_N);
		glm::vec3 specular_lighting = hit_shape->ks * (float)pow(fmax(R_E, 0.0f), hit_shape->n);
		Ray Sray = Ray(surface_point, glm::normalize(scene->lights[i]->position - surface_point));
		float intersection = 1;
		float t_light = glm::dot(scene->lights[i]->position - surface_point, Sray.direction);
		
		// epsilon += 0.000001;
		if((Hit(Sray, epsilon, t_light, scene->shapes) == nullptr)){//nullptr means no hit no shadow
			intersection = 1.0; 
			// cout << "no shadow" << endl;
		}
		else{//shadow 
			intersection = 0.0;
			
		}
		color += (intersection *scene->lights[i]->color * (diffuse_lighting + specular_lighting));
	}

	glm::vec3 reflection = glm::reflect(ray.direction, normal);
	//glm::vec3 reflection = -((2*(glm::dot(ray.origin, normal)))*normal) - ray.origin;
	glm::vec3 refraction = glm::refract(ray.direction, normal, hit_shape->n);
	Ray Rray = Ray(surface_point, reflection);
	Ray Rfray = Ray(surface_point, refraction);
	if((depth < 4)){
		Shape * shape_in_way = Hit(Rray, epsilon, t1, scene->shapes);
		Shape * shape_in_way2 = Hit(Rfray, epsilon, t1, scene->shapes);
		if(shape_in_way != nullptr && shape_in_way->t > epsilon && hit_shape->km.x > 0.0 && hit_shape->km.y > 0.0 && hit_shape->km.z > 0.0){
			// epsilon += 0.001;
			color +=  hit_shape->km*ComputeRayColor(Rray, shape_in_way, scene, (depth+1), t0, t1);
			//color += hit_shape->km*ComputeRayColor(Rfray, shape_in_way2, scene, (depth + 1), t0, t1);
		}
	}
	return color;
}
	

	


// }




void Camera::TakePicture(Scene *scene)
{
	for(int i = 0; i < widthRes; i++){
		for(int j = 0; j < (heightRes); j++){
			//cout << i << " x " << j << " y " << endl;
			Ray camera_ray = CameraRay(i, j);
			float t0 = 0;
			float t1 = 1000.0;
			Shape * hit_shape = Hit(camera_ray, t0, t1, scene->shapes);
			glm::vec3 color = glm::vec3(0.0);
			if(hit_shape != nullptr && (hit_shape->t >= 0)){
				color = ComputeRayColor(camera_ray, hit_shape, scene, 0, t0, t1);
			}
			
			
			//cout << color[0] << " r " << color[1] << " g " << color[2] << " b " << endl;
			renderedImage[(j * widthRes + i) * 3 + 0] = color[0];
			renderedImage[(j * widthRes + i) * 3 + 1] = color[1];
			renderedImage[(j * widthRes + i) * 3 + 2] = color[2];
		}
	}

	


	//memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);
}


