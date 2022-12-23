#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

#include "Program.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
using std::cout;
using std::endl;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define NUM_LIGHTS 2
#define NUM_MATERIALS 3

GLFWwindow *window;

Program program;
Program programs[3];

std::vector<float> posBuff;
std::vector<float> norBuff;
std::vector<float> texBuff;

glm::vec3 eye(0.0f, 0.0f, 4.0f);

struct materialStruct {
	glm::vec3 ka, kd, ks;
	float s;
} materials[NUM_MATERIALS];
materialStruct material = materials[0];
struct lightStruct {
	glm::vec3 position;
	glm::vec3 color;
} lights[NUM_LIGHTS];
lightStruct light = lights[0];
int material_selector = 0;
int light_selector = 0;
// material1.ka = glm::vec3(0.2, 0.2, 0.2);


void Display()
{		
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 modelMatrix(1.0f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.2f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 transformedmodelMatrix(1.0f);
	transformedmodelMatrix = glm::inverse(glm::transpose(modelMatrix));
	program.Bind();
	program.SendUniformData(modelMatrix, "model");
	program.SendUniformData(transformedmodelMatrix, "transformed_model");
	program.SendUniformData(viewMatrix, "view");
	program.SendUniformData(projectionMatrix, "projection");
	program.SendUniformData(eye, "eye");
	program.SendUniformData(material.ka, "ka");
	program.SendUniformData(material.kd, "kd");
	program.SendUniformData(material.ks, "ks");
	program.SendUniformData(material.s, "s");
	program.SendUniformData(lights[0].position, "lights[0].position");
	program.SendUniformData(lights[0].color, "lights[0].color");
	program.SendUniformData(lights[1].position, "lights[1].position");
	program.SendUniformData(lights[1].color, "lights[1].color");
	// program.SendUniformData(lights, "lights");
	glDrawArrays(GL_TRIANGLES, 0, posBuff.size() / 3);
	program.Unbind();
	//cout << light.position.x << " " << light.position.y << " " << light.position.z << endl;
}

// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	switch (key) 
	{
	case 'q':
		glfwSetWindowShouldClose(window, GLFW_TRUE);
		break;
	case '1':
		program = programs[0];
		break;
	case '2':
		program = programs[1];
		break;
	case '3':
		program = programs[2];
		break;
	case 'm':
		if(material_selector < 2){
			material_selector++;
		}
		material = materials[material_selector];
		break;
	case 'M':
		if(material_selector >= 1){
			material_selector--;
		}
		material = materials[material_selector];
		break;	
	case 'l':
		if(light_selector < 1){
			light_selector++;
		}
		light = lights[light_selector];
		break;
	case 'L':
		if(light_selector >= 1){
			light_selector--;
		}
		light = lights[light_selector];
		break;
	case 'x':
		
		light.position.x = light.position.x + 0.10;
		lights[light_selector] = light;
		break;
	case 'y':
		light.position.y = light.position.y + 0.10;
		lights[light_selector] = light;
		break;
	case 'z':
		light.position.z = light.position.z + 0.10;
		lights[light_selector] = light;
		break;
	case 'X':
		light.position.x = light.position.x - 0.10;
		lights[light_selector] = light;
		break;
	case 'Y':
		light.position.y = light.position.y - 0.10;
		lights[light_selector] = light;
		break;
	case 'Z':
		light.position.z = light.position.z - 0.10;
		lights[light_selector] = light;
		break;	
	default:
		break;
	}
}


void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
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

void Init()
{
	glfwInit();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment4 - Ivan Martinez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetCharCallback(window, CharacterCallback);
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	LoadModel("../obj/bunny.obj");
	
	programs[0].SetShadersFileName("../shaders/shader1.vert", "../shaders/shader1.frag");
	programs[0].Init();
	programs[0].SendAttributeData(posBuff, "vPositionModel");
	programs[0].SendAttributeData(norBuff, "vNormalModel");

	programs[1].SetShadersFileName("../shaders/shader2.vert", "../shaders/shader2.frag");
	programs[1].Init();
	// programs[1].SendAttributeData(posBuff, "vPositionModel");
	// programs[1].SendAttributeData(norBuff, "vNormalModel");
	programs[1].SendAttributeData(posBuff, "vPositionModelvary");
	programs[1].SendAttributeData(norBuff, "vNormalModelvary");

	programs[2].SetShadersFileName("../shaders/shader3.vert", "../shaders/shader3.frag");
	programs[2].Init();
	// programs[1].SendAttributeData(posBuff, "vPositionModel");
	// programs[1].SendAttributeData(norBuff, "vNormalModel");
	programs[2].SendAttributeData(posBuff, "vPositionModelvary");
	programs[2].SendAttributeData(norBuff, "vNormalModelvary");

	program = programs[0];
	// program.SendUniformData(eye, "eye");
	materials[0].ka = glm::vec3(0.2, 0.2, 0.2);
	materials[0].kd = glm::vec3(0.8, 0.7, 0.7);
	materials[0].ks = glm::vec3(1.0, 1.0, 1.0);
	materials[0].s = 10.0;

	materials[1].ka = glm::vec3(0.0, 0.2, 0.2);
	materials[1].kd = glm::vec3(0.5, 0.7, 0.2);
	materials[1].ks = glm::vec3(0.1, 1.0, 0.1);
	materials[1].s = 100.0;

	materials[2].ka = glm::vec3(0.2, 0.2, 0.2);
	materials[2].kd = glm::vec3(0.1, 0.3, 0.9);
	materials[2].ks = glm::vec3(0.1, 0.1, 0.1);
	materials[2].s = 1.0;

	lights[0].position = glm::vec3(0.0, 0.0, 3.0);
	lights[0].color = glm::vec3(0.5, 0.5, 0.5);

	lights[1].position = glm::vec3(0.0, 3.0, 0.0);
	lights[1].color = glm::vec3(0.2, 0.2, 0.2);
	material = materials[0];
	light = lights[0];
}


int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}