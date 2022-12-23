#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "Shape.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include <cstring>
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

struct sphereStruct {
	glm::vec3 position;
	float radius;
	glm::vec3 ka, kd, ks, km;
	float n;
} spheres[4];
struct planeStruct {
	glm::vec3 center;
	glm::vec3 normal;
	glm::vec3 ka, kd, ks, km;
	float n;
} planes[2];
struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
}lights[2];
std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;




std::vector<Shape*> shapes;
std::vector<Shape*> shapes2;
std::vector<Light*> lightsources;
std::vector<Light*> lightsources2;

void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Ivan Martinez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
	

	

	Sphere * sphere1 = new Sphere();    //make sure to delete eventually
	Sphere * sphere2 = new Sphere();   //make sure to delete eventually
	Sphere * sphere3 = new Sphere();  //make sure to delete eventually
	Sphere * sphere4 = new Sphere(); //make sure to delete eventually
	// Sphere * sphere5 = new Sphere(); //make sure to delete eventually

	sphere1->position = glm::vec3(-1.0, -0.7, 3.0);
	sphere1->radius = 0.3;
	sphere1->ka = glm::vec3(0.1, 0.1, 0.1);
	sphere1->kd = glm::vec3(0.2, 1.0, 0.2);
	sphere1->ks = glm::vec3(1.0, 1.0, 1.0);
	sphere1->km = glm::vec3(0.0, 0.0, 0.0);
	sphere1->n = 100.0;
	sphere1->type = 1;

	sphere2->position = glm::vec3(1.0, -0.5, 3.0);
	sphere2->radius = 0.5;
	sphere2->ka = glm::vec3(0.1, 0.1, 0.1);
	sphere2->kd = glm::vec3(0.0, 0.0, 1.0);
	sphere2->ks = glm::vec3(1.0, 1.0, 1.0);
	sphere2->km = glm::vec3(0.0, 0.0, 0.0);
	sphere2->n = 10.0;
	sphere2->type = 1;

	sphere3->position = glm::vec3(-1.0, 0.0, 0.0);
	sphere3->radius = 1.0;
	sphere3->ka = glm::vec3(0.0, 0.0, 0.0);
	sphere3->kd = glm::vec3(0.0, 0.0, 0.0);
	sphere3->ks = glm::vec3(0.0, 0.0, 0.0);
	sphere3->km = glm::vec3(1.0, 1.0, 1.0);
	sphere3->n = 0.0;
	sphere3->type = 1;

	sphere4->position = glm::vec3(1.0, 0.0, -1.0);
	sphere4->radius = 1.0;
	sphere4->ka = glm::vec3(0.0, 0.0, 0.0);
	sphere4->kd = glm::vec3(0.0, 0.0, 0.0);
	sphere4->ks = glm::vec3(0.0, 0.0, 0.0);
	sphere4->km = glm::vec3(0.8, 0.8, 0.8);
	sphere4->n = 0.0;
	sphere4->type = 1;

	// sphere5->position = glm::vec3(1.0, 0.0, -1.0);
	// sphere5->radius = 1.0;
	// sphere5->ka = glm::vec3(0.0, 0.0, 0.0);
	// sphere5->kd = glm::vec3(0.0, 0.0, 0.0);
	// sphere5->ks = glm::vec3(0.0, 0.0, 0.0);
	// sphere5->km = glm::vec3(0.8, 0.8, 0.8);
	// sphere5->n = 0.0;

	

	Plane * plane1 = new Plane();  //make sure to delete eventually
	Plane * plane2 = new Plane(); //make sure to delete eventually

	plane1->center = glm::vec3(0.0, -1.0, 0.0);
	plane1->normal = glm::vec3(0.0, 1.0, 0.0);
	plane1->ka = glm::vec3(0.1, 0.1, 0.1);
	plane1->kd = glm::vec3(1.0, 1.0, 1.0);
	plane1->ks = glm::vec3(0.0, 0.0, 0.0);
	plane1->km = glm::vec3(0.0, 0.0, 0.0);
	plane1->n = 0.0;
	plane1->type = 2;

	plane2->center = glm::vec3(0.0, 0.0, -3.0);
	plane2->normal = glm::vec3(0.0, 0.0, 1.0);
	plane2->ka = glm::vec3(0.1, 0.1, 0.1);
	plane2->kd = glm::vec3(1.0, 1.0, 1.0);
	plane2->ks = glm::vec3(0.0, 0.0, 0.0);
	plane2->km = glm::vec3(0.0, 0.0, 0.0);
	plane2->n = 0.0;
	plane2->type = 2;
	
	// plane1->Test();
	Light * light1 = new Light();  //make sure to delete eventually
	Light * light2 = new Light(); //make sure to delete eventually

	light1->position = glm::vec3(0.0, 3.0, -2.0);
	light1->color = glm::vec3(0.2, 0.2, 0.2);

	light2->position = glm::vec3(-2.0, 1.0, 4.0);
	light2->color = glm::vec3(0.5, 0.5, 0.5);

	

	shapes.push_back(sphere1);
	shapes.push_back(sphere2);
	shapes.push_back(sphere3);
	shapes.push_back(sphere4);
	shapes.push_back(plane1);
	shapes.push_back(plane2);

	

	lightsources.push_back(light1);
	lightsources.push_back(light2);

	Triangle * tri = new Triangle();
	tri->ka = glm::vec3(0.1, 0.1, 0.1);
	tri->ks = glm::vec3(0.0, 0.0, 1.0);
	tri->kd = glm::vec3(1.0, 1.0, 0.5);
	tri->km = glm::vec3(0.0, 0.0, 0.0);
	tri->n = 100.0;

// 	v 0.218150 1.177053 -0.100153
// v 0.426353 0.607975 -0.034584
// v 0.184009 1.214551 -0.076804

	tri->vertex0 = glm::vec3(0.218150, 1.177053, -0.100153);
	tri->vertex1 = glm::vec3(0.426353, 0.607975, -0.034584);
	tri->vertex2 = glm::vec3(0.184009, 1.214551, -0.076804);
// 	vn 0.991300 0.129900 0.021100
// vn 0.951800 0.141700 0.272100
// vn 0.132300 0.333500 0.933400
	tri->normal = glm::vec3(0.991300, 0.129900, 0.021100);
	tri->type = 3;


	shapes2.push_back(tri);
	Scene scene = Scene(shapes, lightsources);

	

	Scene scene2 = Scene(shapes2, lightsources);
	glm::vec3 eye = glm::vec3(0.0, 0.0, 7.0);
	glm::vec3 look_at = glm::vec3(0.0, 0.0, 0.0); 
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0); 
	int FovY = 45; 
	int focal_distance = 1;

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, look_at, up, FovY, focal_distance);
	
	auto start = std::chrono::high_resolution_clock::now();
	camera.TakePicture(&scene);
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time passed (ms): " << duration.count() << std::endl;
	
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);

	delete sphere1;
	delete sphere2;
	delete sphere3;
	delete sphere4;
	delete plane1;
	delete plane2;
	delete tri;
}


void LoadModel(char* name)
{
	// Taken from Shinjiro Sueda with slight modification
	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for (size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
					posBuff.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 0]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 1]);
						norBuff.push_back(attrib.normals[3 * idx.normal_index + 2]);
					}
					if (!attrib.texcoords.empty()) {
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 0]);
						texBuff.push_back(attrib.texcoords[2 * idx.texcoord_index + 1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
}


int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}