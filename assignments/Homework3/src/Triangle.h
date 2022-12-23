#pragma once

#include <stdlib.h>
#include <math.h>
#include <vector>

#include <glm/glm.hpp>
#include <vector>
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 1024
//wrapper class for buffer
struct FrameBuffer{
	float color[WINDOW_HEIGHT][WINDOW_WIDTH][3];
	float depth[WINDOW_HEIGHT][WINDOW_WIDTH];
};

class Triangle {
	private:
		glm::vec3 v[3];		// Triangle vertices
		glm::vec3 c[3];		// Vertex color
		glm::vec2 t[3];		// Texture coordinates

	public:
		int type;
		bool transformed = false;
		//int num_of_triangle = 0;


		// Default constructor
		Triangle();

		glm::vec3 getZ(){
			return v[2];
		}

		void setC(glm::vec3 new_c[3]);



		// Constructor without texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& c0, glm::vec3& c1, glm::vec3& c2);
		// Constructor with texture coordinates
		Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2);
		// Rendering the triangle using OpenGL
		void RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool textureMode);
		//transform vectices to screen coordinates 
		glm::vec4 transform_coord(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix,glm::mat4 &viewPort, glm::vec4 vec);
		// Render the triangle on CPU
		void RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix,glm::mat4 &viewPort, bool isTextured, FrameBuffer &buffer, int textureMode, int texWidth, int texHeight, std::vector<float*> texture, int &num);
};
