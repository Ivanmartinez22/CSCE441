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
#include "BVH.h"
#include <cstring>
#include <string.h>

#include<algorithm>
using std::cout;
using std::endl;
using std::string;
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
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
vector<Shape*> triangles;

bool comparex(int a, int b)
{
	// If values are the same then
	// return true
	Triangle * at = static_cast<Triangle*>(triangles[a]);
	Triangle * bt = static_cast<Triangle*>(triangles[b]);
	if (at->average_vertex.x > bt->average_vertex.x) {
		return true;
	}
	return false;
}


bool comparey(int a, int b)
{
	// If values are the same then
	// return true
	Triangle * at = static_cast<Triangle*>(triangles[a]);
	Triangle * bt = static_cast<Triangle*>(triangles[b]);
	if (at->average_vertex.y > bt->average_vertex.y) {
		return true;
	}
	return false;
}



bool comparez(int a, int b)
{
	// If values are the same then
	// return true
	Triangle * at = static_cast<Triangle*>(triangles[a]);
	Triangle * bt = static_cast<Triangle*>(triangles[b]);
	if (at->average_vertex.z > bt->average_vertex.z) {
		return true;
	}
	return false;
}





std::vector<Shape*> shapes;//scene 1
std::vector<Shape*> shapes2;//scene 2 1 bounded box
std::vector<Shape*> shapes3;//scene 3 BVH heirarchy
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
					vertices.push_back(glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												 attrib.vertices[3 * idx.vertex_index + 1],
												 attrib.vertices[3 * idx.vertex_index + 2]));
					if (!attrib.normals.empty()) {
						normals.push_back(glm::vec3(attrib.normals[3 * idx.normal_index + 0],
							attrib.normals[3 * idx.normal_index + 1], attrib.normals[3 * idx.normal_index + 2]));
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
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



	//building Bounded Volume Heirarchy

	vector<BVH*> bvhs;

	LoadModel("../obj/bunny.obj");

	
	glm::vec3 min = vertices[0];
	glm::vec3 max = vertices[0];

	vector<int> indices;
	// vector<glm::vec3> average_vertexes;

 

	for(int i = 0; i < vertices.size()/3;i++){
		Triangle * tri = new Triangle();
		tri->ka = glm::vec3(0.1, 0.1, 0.1);
		tri->kd = glm::vec3(0.0, 0.0, 1.0);
		tri->ks = glm::vec3(1.0, 1.0, 0.5);
		tri->km = glm::vec3(0.0, 0.0, 0.0);
		tri->n = 100.0;

		tri->vertex0 = vertices[i * 3 + 0];  
		tri->vertex1 = vertices[i * 3 + 1];
		tri->vertex2 = vertices[i * 3 + 2];

		
		tri->normal0 = normals[i * 3 + 0];
		tri->normal1 = normals[i * 3 + 1];
		tri->normal2 = normals[i * 3 + 2];
		tri->type = 3;

		triangles.push_back(tri);

		if(vertices[i * 3 + 0].x < min.x){
			min.x = vertices[i * 3 + 0].x;
		}
		if(vertices[i * 3 + 1].y < min.y){
			min.y = vertices[i * 3 + 1].y;
		}
		if(vertices[i * 3 + 2].z < min.z){
			min.z = vertices[i * 3 + 2].z;
		}


		if(vertices[i * 3 + 0].x > max.x){
			max.x = vertices[i * 3 + 0].x;
		}
		if(vertices[i * 3 + 1].y > max.y){
			max.y = vertices[i * 3 + 1].y;
		}
		if(vertices[i * 3 + 2].z > max.z){
			max.z = vertices[i * 3 + 2].z;
		}
		glm::vec3 average_vertex = (vertices[i * 3 + 0]  + vertices[i * 3 + 1] + vertices[i * 3 + 2]) * 0.3333f;
		
		tri->index = i;
		tri->average_vertex = average_vertex;
		indices.push_back(i);
	}

	BVH* bunny_bounding_box = new BVH(); //node root
	bunny_bounding_box->shapes = triangles;
	bunny_bounding_box->indices = indices;
	bunny_bounding_box->max = max;
	bunny_bounding_box->min = min;
	bunny_bounding_box->type = 4;
	bunny_bounding_box->depth = 0;

	BVH* single_bunny_bounding_box = new BVH(); //singular
	single_bunny_bounding_box->shapes = triangles;
	single_bunny_bounding_box->indices = indices;
	single_bunny_bounding_box->max = max;
	single_bunny_bounding_box->min = min;
	single_bunny_bounding_box->type = 4;
	single_bunny_bounding_box->depth = 0;
	single_bunny_bounding_box->isLeaf = true;

	int nodes_count = 0;
	int depth = 0;

	vector<BVH*> pevious_level_of_nodes;
	vector<BVH*> list_of_node_pointers;
	pevious_level_of_nodes.push_back(bunny_bounding_box);
	//cout << indices.size() << endl;

	while(true){

		vector<BVH*> current_level_of_nodes;
		// cout << pevious_level_of_nodes.size() << endl;

		int axis = depth % 3;
		int indices_size;
		// if(axis == 0){// x axis 
		// 	//std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparex);
		// 	//cout << " x " << endl;
		// }
		// else if(axis == 1){// y axis 
		// 	//std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparey);
		// 	//cout << " y " << endl;
			
		// }
		// else if(axis == 2){// z axis 
		// 	//std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparez);
		// 	//cout << " z " << endl;

		// }
		for(int i = 0; i < pevious_level_of_nodes.size(); i++){
			if(axis == 0){// x axis 
				std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparex);
			}
			else if(axis == 1){// y axis 
				std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparey);
				
			}
			else if(axis == 2){// z axis 
				std::sort(pevious_level_of_nodes[i]->indices.begin(),pevious_level_of_nodes[i]->indices.end(), comparez);

			}
			if(pevious_level_of_nodes[i]->indices.size() % 2 > 0){

			}
			int half_size = pevious_level_of_nodes[i]->indices.size() / 2;
			if(pevious_level_of_nodes[i]->indices.size() % 2 > 0){
				half_size += 1;
			}
			std::vector<int> left_indices(pevious_level_of_nodes[i]->indices.begin(), pevious_level_of_nodes[i]->indices.begin() + half_size);
			std::vector<int> right_indices(pevious_level_of_nodes[i]->indices.begin() + (pevious_level_of_nodes[i]->indices.size()/2), pevious_level_of_nodes[i]->indices.end());
			indices_size = half_size;
			vector<Shape*> left_triangles;
			vector<Shape*> right_triangles;
			if(i < 4){
				//cout << "left indexes : " << left_indices.size() << endl;
			//cout << "right indexes : " << right_indices.size() << endl;
			}
			


			for(int j = 0; j < 2; j++){
				if(j == 0){//left
					BVH* left_child_bounding_box = new BVH(); //node root
					// bunny_bounding_box->shapes = triangles;
					left_child_bounding_box->indices = left_indices;
					Triangle* first_tri = static_cast<Triangle*>(triangles[left_indices[0]]);
					glm::vec3 min_vertex_loc = first_tri->vertex0;
					glm::vec3 max_vertex_loc = first_tri->vertex0;
					for(int k = 0; k < left_indices.size(); k++){
						Triangle* iter_tri = static_cast<Triangle*>(triangles[left_indices[k]]);
						left_triangles.push_back(triangles[left_indices[k]]);

						
						//min x, y, z
						if(iter_tri->vertex0.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex0.x;
						}
						if(iter_tri->vertex1.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex1.x;
						}
						if(iter_tri->vertex2.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex2.x;
						}

						if(iter_tri->vertex0.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex0.y;
						}
						if(iter_tri->vertex1.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex1.y;
						}
						if(iter_tri->vertex2.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex2.y;
						}
						
						if(iter_tri->vertex0.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex0.z;
						}
						if(iter_tri->vertex1.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex0.z;
						}
						if(iter_tri->vertex2.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex0.z;
						}

						//max
						if(iter_tri->vertex0.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex0.x;
						}
						if(iter_tri->vertex1.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex1.x;
						}
						if(iter_tri->vertex2.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex2.x;
						}

						if(iter_tri->vertex0.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex0.y;
						}
						if(iter_tri->vertex1.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex1.y;
						}
						if(iter_tri->vertex2.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex2.y;
						}

						if(iter_tri->vertex0.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex0.z;
						}
						if(iter_tri->vertex1.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex1.z;
						}
						if(iter_tri->vertex2.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex2.z;
						}


					}


					left_child_bounding_box->shapes = left_triangles;
					left_child_bounding_box->max = min_vertex_loc;
					left_child_bounding_box->min =  max_vertex_loc;
					left_child_bounding_box->type = 4;
					left_child_bounding_box->depth = (depth + 1);
					
					pevious_level_of_nodes[i]->left = left_child_bounding_box;
					
					list_of_node_pointers.push_back(left_child_bounding_box);
					current_level_of_nodes.push_back(left_child_bounding_box);
					left_child_bounding_box->isLeft = true;
					if(indices_size <= 3){
						left_child_bounding_box->isLeaf = true;
					}



				}
				if(j == 1){//right
					BVH* right_child_bounding_box = new BVH(); //node root
					// bunny_bounding_box->shapes = triangles;
					right_child_bounding_box->indices = right_indices;

					Triangle* first_tri = static_cast<Triangle*>(triangles[right_indices[0]]);
					glm::vec3 min_vertex_loc = first_tri->vertex0;
					glm::vec3 max_vertex_loc = first_tri->vertex0;
					for(int k = 0; k < left_indices.size(); k++){
						Triangle* iter_tri = static_cast<Triangle*>(triangles[right_indices[k]]);
						right_triangles.push_back(triangles[right_indices[k]]);
						//min x, y, z
						if(iter_tri->vertex0.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex0.x;
						}
						if(iter_tri->vertex1.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex1.x;
						}
						if(iter_tri->vertex2.x < min_vertex_loc.x){
							min_vertex_loc.x = iter_tri->vertex2.x;
						}

						if(iter_tri->vertex0.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex0.y;
						}
						if(iter_tri->vertex1.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex1.y;
						}
						if(iter_tri->vertex2.y < min_vertex_loc.y){
							min_vertex_loc.y = iter_tri->vertex2.y;
						}
						
						if(iter_tri->vertex0.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex0.z;
						}
						if(iter_tri->vertex1.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex1.z;
						}
						if(iter_tri->vertex2.z < min_vertex_loc.z){
							min_vertex_loc.z = iter_tri->vertex2.z;
						}

						//max
						if(iter_tri->vertex0.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex0.x;
						}
						if(iter_tri->vertex1.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex1.x;
						}
						if(iter_tri->vertex2.x > max_vertex_loc.x){
							max_vertex_loc.x = iter_tri->vertex2.x;
						}

						if(iter_tri->vertex0.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex0.y;
						}
						if(iter_tri->vertex1.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex1.y;
						}
						if(iter_tri->vertex2.y > max_vertex_loc.y){
							max_vertex_loc.y = iter_tri->vertex2.y;
						}

						if(iter_tri->vertex0.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex0.z;
						}
						if(iter_tri->vertex1.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex1.z;
						}
						if(iter_tri->vertex2.z > max_vertex_loc.z){
							max_vertex_loc.z = iter_tri->vertex2.z;
						}


					}

					right_child_bounding_box->shapes = right_triangles;
					right_child_bounding_box->max = min_vertex_loc;
					right_child_bounding_box->min =  max_vertex_loc;
					right_child_bounding_box->type = 4;
					right_child_bounding_box->depth = (depth + 1);
					right_child_bounding_box->isRight = true;
					if(indices_size <= 3){
						right_child_bounding_box->isLeaf = true;
					}

					pevious_level_of_nodes[i]->right = right_child_bounding_box;
					
					list_of_node_pointers.push_back(right_child_bounding_box);
					current_level_of_nodes.push_back(right_child_bounding_box); 

				}
			}
		}
		//cout << current_level_of_nodes.size() << endl;
		//cout << indices_size << endl;

		depth += 1;

		// int level = depth - 1;
		// int number_of_nodes = pow(2,level);
		// nodes_count += num_nodes;
		

		pevious_level_of_nodes.clear();

		pevious_level_of_nodes = current_level_of_nodes;
		nodes_count += current_level_of_nodes.size();
		if(indices_size <= 3){
			//std::cout << "break" << std::endl;
			break;
		}
	}

	//std::cout << list_of_node_pointers.size() << std::endl;
	bvhs.push_back(bunny_bounding_box);
	shapes2.push_back(single_bunny_bounding_box);
	shapes3.push_back(bunny_bounding_box);

	

// 	v 0.218150 1.177053 -0.100153
// v 0.426353 0.607975 -0.034584
// v 0.184009 1.214551 -0.076804

	// cout << vertices.size() << endl;	
	// cout << normals.size() << endl;

	// shapes2.push_back(tri);


	//first scene 
	Scene scene = Scene(shapes, lightsources);

	//second scene - 1 bounded box
	Scene scene2 = Scene(shapes2, lightsources);


	//third scene - bounded box heirarchy
	Scene scene3 = Scene(shapes3, lightsources);
	string response;
	while(true){
		cout << "Select scene, type a number(1 == Spheres and Planes, 2 == 1 Bounded Box Bunny, 3 == BVH Heirarchy Bunny): ";
		response = "";
		std::cin >> response;
		cout << endl;
		if(response == "1" || response == "2" || response == "3"){
			 break;
		}
		else{
			cout << "Invalid response, try again" << endl;
		}   

	}

	if(response == "1"){
		cout << "Loading Scene 1" << endl;
	}
	else if(response == "2"){
		cout << "Loading Scene 2" << endl;
		scene = scene2;
	}
	else if(response == "3"){
		cout << "Loading Scene 3" << endl;
		scene = scene3;
	}
	
	

	
	
	glm::vec3 eye = glm::vec3(0.0, 0.0, 7.0);
	glm::vec3 look_at = glm::vec3(0.0, 0.0, 0.0); 
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0); 
	int FovY = 45; 
	int focal_distance = 1;

	Camera camera = Camera(WINDOW_WIDTH, WINDOW_HEIGHT, eye, look_at, up, FovY, focal_distance);
	camera.triangles = triangles;
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
	// delete tri;
	
	for(int i = 0; i < triangles.size(); i++){
		delete triangles[i];
	}

	delete bunny_bounding_box;

	for(int i = 0; i < list_of_node_pointers.size(); i++){
		//cout <<" max: " << list_of_node_pointers[i]->max.x << " x " << list_of_node_pointers[i]->max.y << " y " << list_of_node_pointers[i]->max.z << " x " << endl;
		//cout <<" min: "  << list_of_node_pointers[i]->min.x << " x " << list_of_node_pointers[i]->min.y << " y " << list_of_node_pointers[i]->min.z << " x " << endl;
		
		delete list_of_node_pointers[i];
	}

	// delete[] bvh_nodes;
	
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