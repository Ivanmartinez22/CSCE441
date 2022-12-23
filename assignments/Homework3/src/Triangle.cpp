#include "Triangle.h"
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <iostream>

#include <glm/gtx/string_cast.hpp>
using std::cout;
using std::endl;
using std::vector;

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))
float range(float input){
	float num = input;
	if(input > 1.00){
		num = (input - 1);
	}
	else if(input < 0.00){
		num = (1 + input);
	}
	return num;
}

float wrap_around(float input, float maxLength){
	float num = input;
	if(input > maxLength){
		num = (input - maxLength);
		
		
	}
	else if(input < 0.00){
		num = (maxLength + input);
		
	}
	return num;
}

glm::vec3 lerp(float x, glm::vec3 v0, glm::vec3 v1){

	glm::vec3 meta_v0 = v0;
	meta_v0.x *= x;
	meta_v0.y *= x;
	meta_v0.z *= x;
	glm::vec3 meta_v1 = v1;
	meta_v1.x *= x;
	meta_v1.y *= x;
	meta_v1.z *= x;
	return v0 + (meta_v1 - meta_v0);
}

glm::vec3 tex_color(std::vector<float*> texture, int texWidth, int x, int y){
	
	glm::vec3 color;
	int coord1 = int(y) * texWidth * 3 + int(x) * 3 + 0;
	int coord2 = int(y) * texWidth * 3 + int(x) * 3 + 1;
	int coord3 = int(y) * texWidth * 3 + int(x) * 3 + 2;
	color.x = texture[0][coord1];
	color.y = texture[0][coord2];
	color.z = texture[0][coord3];

	return color;
}

glm::vec3 tex_color_at_level(std::vector<float*> texture, int texWidth, int x, int y, int L){
	
	glm::vec3 color;
	int coord1 = int(y) * texWidth * 3 + int(x) * 3 + 0;
	int coord2 = int(y) * texWidth * 3 + int(x) * 3 + 1;
	int coord3 = int(y) * texWidth * 3 + int(x) * 3 + 2;
	color.x = texture[L][coord1];
	color.y = texture[L][coord2];
	color.z = texture[L][coord3];
	return color;
}


bool bound_check(float x, float y, int texWidth, int texHeight){
	if((x>= 0) && (y>=0) && (x <= (texWidth-1)) && (y <= (texHeight-1))){
		return true;
	}
	return false;
}

glm::vec3 nearest_neighbor(float x, float y, float texWidth, float texHeight, std::vector<float*> texture, float L){
	x *= texWidth;
	y *= texHeight;
	x = wrap_around(x, texWidth);
	y = wrap_around(y, texHeight);
	x = floor(x);
	y = floor(y);
	if(bound_check(x, y, texWidth, texHeight)){
		
		glm::vec3 color = tex_color_at_level(texture, texWidth, x, y, L);
		
		return color;
	}
	
	glm::vec3 black;
	black.x = 0.0;
	black.y = 0.0;
	black.z = 0.0;
	return black;

}

glm::vec3 bilinear(float x, float y, float texWidth, float texHeight, std::vector<float*> texture, float L){

	float u = x;
	float v = y;
	x *= texWidth;
	y *= texHeight;
	x = wrap_around(x, texWidth);
	y = wrap_around(y, texHeight);

	
	int x1 = floor(x);
	int y1 = floor(y);
	int x2 = ceil(x);
	int y2 = ceil(y);
	int x3 = ceil(x);
	int y3 = floor(y);
	int x4 = floor(x);
	int y4 = ceil(y);

	if(bound_check(x1, y1, texWidth, texHeight) && bound_check(x2, y2, texWidth, texHeight) && bound_check(x3, y3, texWidth, texHeight) && bound_check(x4, y4, texWidth, texHeight) && bound_check(x, y, texWidth, texHeight)){
		glm::vec3 u00color = tex_color_at_level(texture, texWidth, x1, y1, L);
		
		glm::vec3 u01color = tex_color_at_level(texture, texWidth, x4, y4, L);
		
		glm::vec3 u11color = tex_color_at_level(texture, texWidth, x2, y2, L);
		
		glm::vec3 u10color = tex_color_at_level(texture, texWidth, x3, y3, L);
		
		
		float t = y - y1;
		float s = x - x1;

		
		glm::vec3 u0 = lerp(s, u00color, u10color);
		glm::vec3 u1 = lerp(s, u01color, u11color);
		glm::vec3 color = lerp(t, u0, u1);
		
		return color;
	}
	
	glm::vec3 color = nearest_neighbor(u, v, texWidth, texHeight, texture, L);
	return color;
	
}


// float max_num(float a, float b, float c) 
// { 
// 	if((a > b) && (a > c)){
// 		return a;
// 	}
// 	else if((b > a) && (b > c)){
// 		return b;
// 	}
// 	else if((c > a) && (c > b)){
// 		return c;
// 	}
// } 

// float min_num(float a, float b, float c) 
// { 
// 	if((a < b) && (a < c)){
// 		return a;
// 	}
// 	else if((b < a) && (b < c)){
// 		return b;
// 	}
// 	else if((c < a) && (c < b)){
// 		return c;
// 	}
// } 
float max_num(float x, float y, float z){
  return x > y ? (x > z ? x : z) : (y > z ? y : z);
}
float min_num(float x, float y, float z){
  return x < y ? (x < z ? x : z) : (y < z ? y : z);
}

float max_num(float a, float b) 
{ 
	if(a > b){
		return a;
	}
	else if(b > a){
		return b;
	}
	return a;
} 


Triangle::Triangle()
{
	v[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	c[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	c[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);
}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& c0, glm::vec3& c1, glm::vec3& c2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	//white color scheme 
	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

	//rgb colors
	// c[0] = glm::vec3(1.0f, 0.0f, 0.0f);
	// c[1] = glm::vec3(0.0f, 0.0f, 1.0f);
	// c[2] = glm::vec3(0.0f, 1.0f, 0.0f);

	c[0] = c0;
	c[1] = c1;
	c[2] = c2;

	t[0] = glm::vec2(0.0f, 0.0f);
	t[1] = glm::vec2(0.0f, 0.0f);
	t[2] = glm::vec2(0.0f, 0.0f);

};

void Triangle::setC(glm::vec3 new_c[3]){
	c[0] = new_c[0];
	c[1] = new_c[1];
	c[2] = new_c[2];
}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec2& t0, glm::vec2& t1, glm::vec2& t2)
{
	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	t[0] = t0;
	t[1] = t1;
	t[2] = t2;

	c[0] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[1] = glm::vec3(1.0f, 1.0f, 1.0f);
	c[2] = glm::vec3(1.0f, 1.0f, 1.0f);

};

// Rendering the triangle using OpenGL
void Triangle::RenderOpenGL(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, bool isTextured)
{

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glm::value_ptr(modelViewMatrix));

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(projectionMatrix));
	
	// For textured object
	if (isTextured)
	{
		//cout << "texture vertex " << endl;
		glEnable(GL_TEXTURE_2D);

		// Avoid modulating the texture by vertex color
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glBegin(GL_TRIANGLES);

			glTexCoord2f(t[0].x, t[0].y);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glTexCoord2f(t[1].x, t[1].y);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glTexCoord2f(t[2].x, t[2].y);
			glVertex3f(v[2].x, v[2].y, v[2].z);

		glEnd();

		glDisable(GL_TEXTURE_2D);


	}
	// For object with only vertex color
	else
	{
		// cout << "vertex color " << endl;
		glBegin(GL_TRIANGLES);

			glColor3f(c[0].x, c[0].y, c[0].z);
			glVertex3f(v[0].x, v[0].y, v[0].z);

			glColor3f(c[1].x, c[1].y, c[1].z);
			glVertex3f(v[1].x, v[1].y, v[1].z);

			glColor3f(c[2].x, c[2].y, c[2].z);
			glVertex3f(v[2].x, v[2].y, v[2].z);
		
		glEnd();
	}

}

glm::vec4 Triangle::transform_coord(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, glm::mat4 &viewPort, glm::vec4 vec){
	
	glm::mat4 matrix = viewPort * projectionMatrix * modelViewMatrix;
	glm::vec4 return_vec = matrix * vec; 
	
	
	return_vec.x = return_vec.x/return_vec.w;
	return_vec.y = return_vec.y/return_vec.w;
	return_vec.z = return_vec.z/return_vec.w;

	return return_vec;
}



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

// rasterization not working correctly - inside check function incorrect 
// Render the triangle on CPU
void Triangle::RenderCPU(glm::mat4 &modelViewMatrix, glm::mat4 &projectionMatrix, glm::mat4 &viewPort  ,bool isTextured, FrameBuffer &buffer, int textureMode,  int texWidth, int texHeight, std::vector<float*> texture, int &num)
{
	
	glm::vec4 v1 = glm::vec4(this->v[0].x, this->v[0].y, this->v[0].z, 1.0);
	glm::vec4 v2 = glm::vec4(this->v[1].x, this->v[1].y, this->v[1].z, 1.0);
	glm::vec4 v3 = glm::vec4(this->v[2].x, this->v[2].y, this->v[2].z, 1.0);

	v1 = transform_coord(modelViewMatrix, projectionMatrix, viewPort, v1);
	v2 = transform_coord(modelViewMatrix, projectionMatrix, viewPort, v2);
	v3 = transform_coord(modelViewMatrix, projectionMatrix, viewPort, v3);
	glm::vec2 t1 = glm::vec2(this->t[0].x, this->t[0].y);
	glm::vec2 t2 = glm::vec2(this->t[1].x, this->t[1].y);
	glm::vec2 t3 = glm::vec2(this->t[2].x, this->t[2].y);
	//bounding box coordinates 
	// vector<float> x_values;
	// x_values.push_back(v1.x);
	// x_values.push_back(v2.x);
	// x_values.push_back(v3.x);
	// vector<float> y_values;
	// y_values.push_back(v1.y);
	// y_values.push_back(v2.y);
	// y_values.push_back(v3.y);
	// float x_min = *min_element(x_values.begin(), x_values.end());
	// float x_max = *max_element(x_values.begin(), x_values.end());
	// float y_min = *min_element(y_values.begin(), y_values.end());
	// float y_max = *max_element(y_values.begin(), y_values.end());
	float x_min = min_num(v1.x, v2.x, v3.x);
	float y_min = min_num(v1.y, v2.y, v3.y);
	float x_max = max_num(v1.x, v2.x, v3.x);
	float y_max = max_num(v1.y, v2.y, v3.y);

	
	
	for(int i = int(x_min); i <= int(x_max); i++){
		for(int j = int(y_min); j <= int(y_max); j++){
			if((i < WINDOW_HEIGHT) && ((i > 0)) && (j < WINDOW_WIDTH) && ((j > 0))){
				float alpha_var = alpha(i, j, v1.x, v2.x, v3.x, v1.y, v2.y, v3.y);
				float beta_var = beta(i, j, v1.x, v2.x, v3.x, v1.y, v2.y, v3.y);
				float gamma_var = gamma(alpha_var, beta_var);
				
				if((beta_var >= 0) && (beta_var <= 1) && (alpha_var >= 0) && (alpha_var  <= 1) && (gamma_var >= 0) && (gamma_var  <= 1)){

					
					//values for texture coordinates 

					float z = alpha_var*v1.z + beta_var*v2.z + gamma_var*v3.z;

					if(isTextured){
						float z_inv_v1 = 1/v1.z;
						float z_inv_v2 = 1/v2.z;
						float z_inv_v3 = 1/v3.z;
						glm::vec2 q_sca_t1 = t1/v1.z;
						glm::vec2 q_sca_t2 = t2/v2.z;
						glm::vec2 q_sca_t3 = t3/v3.z;

						float z_inv_at_pixel = alpha_var*z_inv_v1 + beta_var*z_inv_v2 + gamma_var*z_inv_v3;
						glm::vec2 texture_at_pixel = alpha_var*q_sca_t1 + beta_var*q_sca_t2 + gamma_var*q_sca_t3;
						texture_at_pixel /= z_inv_at_pixel;
						
						float x = texture_at_pixel.x;
						float y = texture_at_pixel.y;
						if(textureMode == 0){ // nearest neighbor 
							
							if(z < buffer.depth[j][i]){
								glm::vec3 color = nearest_neighbor(x, y, texWidth, texHeight, texture, 0);
									buffer.color[j][i][0] = color.x;
									buffer.color[j][i][1] = color.y;
									buffer.color[j][i][2] = color.z;
									buffer.depth[j][i] = z;
								
							
							}
							
							

						}
						else if(textureMode == 1){ // bilinear interpolation

							if(z < buffer.depth[j][i]){
								glm::vec3 color = bilinear(x, y, texWidth, texHeight, texture, 0);
									buffer.color[j][i][0] = color.x;
									buffer.color[j][i][1] = color.y;
									buffer.color[j][i][2] = color.z;
									buffer.depth[j][i] = z;
								
								
							
							}
						}
						else if(textureMode == 2){ // mipmapping
							float u = x;
							float v = y;
							x *= texWidth;
							y *= texHeight;
							
								glm::vec2 uv00;
								uv00.x = x;
								uv00.y = y;
								glm::vec2 uv01 = uv00;
								uv01.y += 1;
								glm::vec2 uv10 = uv00;
								uv10.x += 1;

								float dudx = uv10.x - uv00.x;
								float dvdx = uv10.y - uv00.y;


								float dudy = uv01.x - uv00.x;
								float dvdy = uv01.y - uv00.y;
								dudx = dudx * dudx;
								dvdx = dvdx * dvdx;
								dudy = dudy * dudy;
								dvdy = dvdy * dvdy;
								float left = dudx + dvdx;
								float right = dudy + dvdy;
								left = sqrt(left);
								right = sqrt(right);
								// vector<float> nums;
								// nums.push_back(left);
								// nums.push_back(right);
								// float L = *max_element(nums.begin(), nums.end());
								float L = max_num(left, right);
								float d = log2(L);
								int D = round(d);

								
								
								int upper = D;
								int lower = D + 1;



							
								glm::vec3 lower_color = bilinear(u, v, texWidth, texHeight, texture, lower);
								glm::vec3 upper_color = bilinear(u, v, texWidth, texHeight, texture, upper);

								
								glm::vec3 color = lerp(d, lower_color, upper_color);
							
								if(z < buffer.depth[j][i]){
									buffer.color[j][i][0] = upper_color.x;
									buffer.color[j][i][1] = upper_color.y;
									buffer.color[j][i][2] = upper_color.z;
									buffer.depth[j][i] = z;

							
								}
						}

					}
					else{
						glm::vec3 color = alpha_var*c[0] + beta_var*c[1] + gamma_var*c[2];
						if(z < buffer.depth[j][i]){
							buffer.color[j][i][0] = color.x;
							buffer.color[j][i][1] = color.y;
							buffer.color[j][i][2] = color.z;
							buffer.depth[j][i] = z;
						}
					}
				}
			}
		}
	}
}