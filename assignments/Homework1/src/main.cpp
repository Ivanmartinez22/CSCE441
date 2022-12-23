#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include <utility>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

#define WINDOW_WIDTH 900
#define WINDOW_HEIGHT 600

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
bool mask[WINDOW_HEIGHT][WINDOW_WIDTH];


GLFWwindow *window;

//access and control variables 
double x_pos_access = 0;
double y_pos_access = 0;

int mouse_lbuuton_state = 0;

int size_of_brush = 1;



// Color structure. Can be used to define the brush and background color.
struct color { float r, g, b; };

struct coordinate { int x, y; };

color background = {0,0,0};
color brush_color = {0,0,0};

// A function clamping the input values to the lower and higher bounds
#define CLAMP(in, low, high) ((in) < (low) ? (low) : ((in) > (high) ? (high) : in))

//converts decimals to binary
vector<int> dec_to_bin(char n){
	vector<int> result;
	if((n == '0') || (n == ')')){
		result.push_back(0);
		result.push_back(0);
		result.push_back(0);
	}
	if((n == '1') || (n == '!')){
		result.push_back(0);
		result.push_back(0);
		result.push_back(1);
	}
	if((n == '2') || (n == '@')){
		result.push_back(0);
		result.push_back(1);
		result.push_back(0);
	}
	if((n == '3') || (n == '#')){
		result.push_back(0);
		result.push_back(1);
		result.push_back(1);
	}
	if((n == '4') || (n == '$')){
		result.push_back(1);
		result.push_back(0);
		result.push_back(0);
	}
	if((n == '5') || (n == '%')){
		result.push_back(1);
		result.push_back(0);
		result.push_back(1);
	}
	if((n == '6') || (n == '^')){
		result.push_back(1);
		result.push_back(1);
		result.push_back(0);
	}
	if((n == '7') || (n == '&')){
		result.push_back(1);
		result.push_back(1);
		result.push_back(1);
	}
	return result;
}

// Set a particular pixel of the frameBuffer to the provided color
void SetFrameBufferPixel(int x, int y, struct color lc)
{
	// Origin is upper left corner. 
	// Changes the origin from the lower-left corner to the upper-left corner
	y = WINDOW_HEIGHT - 1 - y;

	x = CLAMP(x, 0, WINDOW_WIDTH - 1);
	y = CLAMP(y, 0, WINDOW_HEIGHT - 1);
	// cout << "frame buffer access 1" << endl;
	frameBuffer[y][x][0] = lc.r;
	frameBuffer[y][x][1] = lc.g;
	frameBuffer[y][x][2] = lc.b;
	

}

void ClearFrameBuffer()
{
	memset(frameBuffer, 0.0f, sizeof(float) * WINDOW_WIDTH * WINDOW_HEIGHT * 3);
}

// Display frameBuffer on screen
void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		mouse_lbuuton_state = GLFW_PRESS;
		x_pos_access = xpos;
		y_pos_access = ypos; 
		// std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
	}
}


vector<coordinate> brush_function(double xpos, double ypos, int size, vector<coordinate> &coordinates){
	// cout << "x: " << xpos << " y: " << ypos << endl;
	// cout << "start" << endl;
	for(int j = (ypos - size); j <= (ypos + size); j++){
		
		for(int i = (xpos - size); i <= (xpos + size); i++){
			coordinate iterative_point;
			iterative_point.x = i;
			iterative_point.y = j;
			// cout << i << " x " << j << " y " << endl;
			if((i >= 0) && (i < 900) && (j >= 0) && (j < 600))
				coordinates.push_back(iterative_point);
		}
	}
	// cout << "end" << endl;

	return coordinates;
	

}

// vector<coordinate> circle_brush_function(double xpos, double ypos, int radius, vector<coordinate> &coordinates){
// 	for(int j = (ypos - size); j <= (ypos + size); j++){
// 		for(int i = (xpos - size); i <= (xpos + size); i++){
// 			coordinate iterative_point;
// 			iterative_point.x = i;
// 			iterative_point.y = j;
// 			coordinates.push_back(iterative_point);
// 		}
// 	}

// 	return coordinates;
	

// }

void draw_brush(vector<coordinate> coordinates, color c){
	int size = coordinates.size();
	for(int i = 0; i < size; i++){
		if((coordinates[i].x >= 0) && (coordinates[i].x < 900) && (coordinates[i].y >= 0) && (coordinates[i].y < 600)){ //making sure drawing is not out of bounds 

			SetFrameBufferPixel(coordinates[i].x, coordinates[i].y, c);
			// cout << "mask access 1" << endl;
			mask[coordinates[i].y][coordinates[i].x] = true; 
		}
		
	}

}

void draw_background(){
	for(int i = 0; i < 600; i++){
		for(int j = 0; j < 900; j++){
			if(!mask[i][j]){
				SetFrameBufferPixel(j,i, background);
			}
		}
	}
}

void clear(){
	for(int i = 0; i < 600; i++){
		for(int j = 0; j < 900; j++){
			SetFrameBufferPixel(j,i, background);
			mask[i][j] = false; 
			// cout << "mask access 2" << endl;
		}
	}
}



// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
	{
		std::cout << "Mouse left button is pressed." << std::endl;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) 
	{
		std::cout << "Mouse right button is pressed." << std::endl;
		clear();
	}

}

// You can use "switch" or "if" to compare key to a specific character.
// for example,
// if (key == '0')
//     DO SOMETHING

// Keyboard callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	if ((key == '+') && (size_of_brush < 128)){
		size_of_brush *= 2; 
		cout << "brush size " << size_of_brush << endl;
	}
	if ((key == '-') && (size_of_brush > 1)){
		size_of_brush /= 2; 
		cout << "brush size " << size_of_brush << endl;
	}
	if(key == '0'){
		vector<int> rgb_values = dec_to_bin('0');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '1'){
		vector<int> rgb_values = dec_to_bin('1');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '2'){
		vector<int> rgb_values = dec_to_bin('2');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '3'){
		vector<int> rgb_values = dec_to_bin('3');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '4'){
		vector<int> rgb_values = dec_to_bin('4');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '5'){
		vector<int> rgb_values = dec_to_bin('5');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '6'){
		vector<int> rgb_values = dec_to_bin('6');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == '7'){
		vector<int> rgb_values = dec_to_bin('7');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		brush_color = new_color;
	}
	if(key == ')'){
		vector<int> rgb_values = dec_to_bin('0');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '!'){
		vector<int> rgb_values = dec_to_bin('1');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '@'){
		vector<int> rgb_values = dec_to_bin('2');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '#'){
		vector<int> rgb_values = dec_to_bin('3');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '$'){
		vector<int> rgb_values = dec_to_bin('4');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '%'){
		vector<int> rgb_values = dec_to_bin('5');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '^'){
		vector<int> rgb_values = dec_to_bin('6');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}
	if(key == '&'){
		vector<int> rgb_values = dec_to_bin('7');
		color new_color = {rgb_values[0], rgb_values[1], rgb_values[2]};
		background = new_color;
		draw_background();
	}

	
}

void Init()
{
	for(int i = 0; i < WINDOW_HEIGHT; i++){
		for(int j = 0; j < WINDOW_WIDTH; j++){
			mask[i][j] = false;
		}
	}
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment 1 - Ivan Martinez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);
	glewExperimental = GL_TRUE;
	glewInit();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();
}


int main()
{	

	Init();
	while (glfwWindowShouldClose(window) == 0)
	{
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
		vector<coordinate> coordinates;
	
		brush_function(x_pos_access, y_pos_access, size_of_brush, coordinates);
		if(mouse_lbuuton_state == GLFW_PRESS){
			
			
			// cout << coordinates.size() << " coordinates generated" << endl;
			// for(int i = 0; i < coordinates.size(); i++){
			// 	cout << coordinates[i].x << " X " << coordinates[i].y << " Y "; 
			// }
			draw_brush(coordinates, brush_color);
		}
		mouse_lbuuton_state = 0;
		coordinates.clear();

	}

	glfwTerminate();
	return 0;
}