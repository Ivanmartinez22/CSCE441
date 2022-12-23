#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <iostream>
#include "MatrixStack.h"
#include "Program.h"
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;



#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

char* vertShaderPath = "../shaders/shader.vert";
char* fragShaderPath = "../shaders/shader.frag";

GLFWwindow *window;
double previousXpos = 400, previousYpos = 400;
double currentXpos, currentYpos;

bool first_use_of_mouse = true;



//camera control variables 
glm::vec3 eye(0.0f, 0.0f, 8.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);
glm::vec3 global_up(0.0f, 1.0f, 0.0f);
glm::vec3 global_right(1.0f, 0.0f, 0.0f);



//Vertex Shading program/files 
Program program;

//Matrix Stack as seen in notes 
MatrixStack modelViewProjectionMatrix;

// Draw cube on screen
void DrawCube(glm::mat4& modelViewProjectionMatrix)
{
	program.SendUniformData(modelViewProjectionMatrix, "mvp");
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

class RobotElements
{
	private:
		

		//children nodes/components 
		RobotElements *left, *right, *child;

		//parent node/component 
		RobotElements *parent;

		//A glm::vec3 representing the translation of the component’s joint with respect to the parent components joint.
		glm::vec3 translation_of_self_by_parent_joint; // connecting to parent 
		//A glm::vec3 representing the current joint angles about the X, Y, and Z axes of the component’s joint. (You may want to start with Z-rotations only.)
		glm::vec3 joint_angles;
		//A glm::vec3 representing the translation of the component with respect to its joint. // moves center coord
		glm::vec3 translation_of_self_by_own_joint;
		//A glm::vec3 representing the X, Y, and Z scaling factors for the component.
		glm::vec3 scaling_factor;

		//center position
		glm::vec3 center_position;

		string element_name;

		

	public:
		bool isHead = false;
		bool isOrganzingElement = false;
		bool isSelected = false;
		RobotElements() {
			left = nullptr;
			right = nullptr;
			parent = nullptr;
			child = nullptr;
			element_name = "";
			joint_angles = glm::vec3(0,0,0);

		}
		~RobotElements() {}


		//set left, right, and parent elements 
		void set_left(RobotElements *element);
		void set_right(RobotElements *element);
		void set_child(RobotElements *element);
		void set_parent(RobotElements *element);
		//set vectors 
		void set_translation_of_self_by_parent_joint(glm::vec3 vec);
		void set_joint_angles(glm::vec3 vec);
		void set_translation_of_self_by_own_joint(glm::vec3 vec);
		void set_scaling_factor(glm::vec3 vec);
		// void set_center_position(glm::vec3 vec);
		//set name 
		void set_name(string name);
		//recursive drawing function
		void draw_self_and_children(RobotElements* element);

		//getters 
		string get_name();
		RobotElements* getRight(){
			return right;
		};
		RobotElements* getLeft(){
			return left;
		};

		RobotElements* getChild(){
			return child;
		}
		RobotElements* getParent(){
			return parent;
		};
		glm::vec3 get_translation_of_self_by_parent_joint();
		glm::vec3 get_joint_angles();
		glm::vec3 get_translation_of_self_by_own_joint();
		glm::vec3 get_scaling_factor();
		
};

void RobotElements::draw_self_and_children(RobotElements* element){

	//traverses and draws all elements recursively in preorder 
	if(element != nullptr){
		if(!element->isOrganzingElement){
			 	modelViewProjectionMatrix.pushMatrix();
				modelViewProjectionMatrix.translate(element->get_translation_of_self_by_parent_joint());
				modelViewProjectionMatrix.translate(element->get_translation_of_self_by_own_joint());
				modelViewProjectionMatrix.rotateX(glm::radians(element->get_joint_angles().x));
				modelViewProjectionMatrix.rotateY(glm::radians(element->get_joint_angles().y));
				modelViewProjectionMatrix.rotateZ(glm::radians(element->get_joint_angles().z));
				glm::vec3 inverse_own_joint; 
				inverse_own_joint.x = -1 * element->get_translation_of_self_by_own_joint().x;
				inverse_own_joint.y = -1 * element->get_translation_of_self_by_own_joint().y;
				inverse_own_joint.z = -1 * element->get_translation_of_self_by_own_joint().z;
				
				modelViewProjectionMatrix.translate(inverse_own_joint);
				
				modelViewProjectionMatrix.pushMatrix();
				glm::vec3 temp_scale = element->get_scaling_factor();
				if(element->isSelected){
						
						// cout << element->get_name() << endl;
						float scale = 1.0;
						if(element->get_name() == "torso"){
							scale += 0.05;
						}
						else{
							scale += 0.3;
						}
						temp_scale.x *= scale;
						temp_scale.y *= scale;
						temp_scale.z *= scale;
					}
				modelViewProjectionMatrix.scale(temp_scale);
				DrawCube(modelViewProjectionMatrix.topMatrix());
				modelViewProjectionMatrix.popMatrix();
		}
		draw_self_and_children(element->getChild());
		draw_self_and_children(element->getLeft());
		draw_self_and_children(element->getRight());
		if(!element->isOrganzingElement){
			modelViewProjectionMatrix.popMatrix();
		}
		
	}
}

void RobotElements::set_left(RobotElements *element){
	this->left = element;
}

void RobotElements::set_right(RobotElements *element){
	this->right = element;
}

void RobotElements::set_child(RobotElements *element){
	this->child = element;
}

void RobotElements::set_parent(RobotElements *element){
	this->parent = element;
}

void RobotElements::set_name(string name){
	this->element_name = name;
}
string RobotElements::get_name(){
	return this->element_name; 
}

void RobotElements::set_translation_of_self_by_parent_joint(glm::vec3 vec){
	this->translation_of_self_by_parent_joint = vec;
}

void RobotElements::set_translation_of_self_by_own_joint(glm::vec3 vec){
	this->translation_of_self_by_own_joint = vec;
}

void RobotElements::set_joint_angles(glm::vec3 vec){
	this->joint_angles = vec;
}

void RobotElements::set_scaling_factor(glm::vec3 vec){
	this->scaling_factor = vec;
}

glm::vec3 RobotElements::get_translation_of_self_by_parent_joint(){
	return this->translation_of_self_by_parent_joint;
}

glm::vec3 RobotElements::get_translation_of_self_by_own_joint(){
	return this->translation_of_self_by_own_joint;
}

glm::vec3 RobotElements::get_joint_angles(){
	return this->joint_angles;
}

glm::vec3 RobotElements::get_scaling_factor(){
	return this->scaling_factor;
}

//global robot elements 

vector<RobotElements*> elements_in_order;

int size_of_elements_vector;

int selector_index = -1;  


//Global robot elements 
//Head and torso 
RobotElements head;
RobotElements torso;

// //left and right side of body 
// RobotElements left;
// RobotElements right;

RobotElements arms;
RobotElements legs;

//arms 
RobotElements left_upper_arm;
RobotElements left_lower_arm;
RobotElements right_upper_arm;
RobotElements right_lower_arm;

//legs 
RobotElements right_upper_leg;
RobotElements right_lower_leg;
RobotElements left_upper_leg;
RobotElements left_lower_leg;

RobotElements* selected_element;
RobotElements unselect; 

//traverses graph in preorder and adds elements to vector in proper order, excludes head 
void preorder(RobotElements* element, vector<RobotElements*> &list){
	if(element != nullptr){
		if(!element->isOrganzingElement){
			list.push_back(element);
		}
			
		preorder(element->getLeft(), list);
		preorder(element->getRight(), list);
	}
		
}

void ConstructRobot()
{

	// scaling of elements 
	glm::vec3 scale_head = glm::vec3(0.25f, 0.25f, 0.25f);
	glm::vec3 scale_torso = glm::vec3(0.6f, 1.2f, 0.3f);
	glm::vec3 scale_upper_arm = glm::vec3(0.6f, 0.17f, 0.17f);
	glm::vec3 scale_lower_arm = glm::vec3(0.5f, 0.15f, 0.15f);
	glm::vec3 scale_upper_leg = glm::vec3(0.25f, 0.6f, 0.2f);
	glm::vec3 scale_lower_leg = glm::vec3(0.20f, 0.6f, 0.175f);

	unselect.set_name("none selected");
	
	//seting translation vertices 
	head.set_translation_of_self_by_parent_joint(glm::vec3(0.0f, 1.4f, 0.0f));
	head.set_translation_of_self_by_own_joint(glm::vec3(0.0f, -0.7f, 0.0f));

	left_upper_arm.set_translation_of_self_by_parent_joint(glm::vec3(-1.2f, 1.0f, 0.0f));
	right_upper_arm.set_translation_of_self_by_parent_joint(glm::vec3(1.2f, 1.0f, 0.0f));
	
	left_upper_arm.set_translation_of_self_by_own_joint(glm::vec3(0.6f, 0.0f, 0.0f));
	right_upper_arm.set_translation_of_self_by_own_joint(glm::vec3(-0.6f, 0.0f, 0.0f));

	left_lower_arm.set_translation_of_self_by_parent_joint(glm::vec3(-1.1f, 0.0f, 0.0f));
	right_lower_arm.set_translation_of_self_by_parent_joint(glm::vec3(1.1f, 0.0f, 0.0f));

	left_lower_arm.set_translation_of_self_by_own_joint(glm::vec3(0.55f, 0.0f, 0.0f));
	right_lower_arm.set_translation_of_self_by_own_joint(glm::vec3(-0.55f, 0.0f, 0.0f));

	left_upper_leg.set_translation_of_self_by_parent_joint(glm::vec3(-0.3f, -1.8f, 0.0f));
	right_upper_leg.set_translation_of_self_by_parent_joint(glm::vec3(0.3f, -1.8f, 0.0f));
	
	left_upper_leg.set_translation_of_self_by_own_joint(glm::vec3(0.0f, 0.9f, 0.0f));
	right_upper_leg.set_translation_of_self_by_own_joint(glm::vec3(0.0f, 0.9f, 0.0f));

	left_lower_leg.set_translation_of_self_by_parent_joint(glm::vec3(0.0f, -1.2f, 0.0f));
	right_lower_leg.set_translation_of_self_by_parent_joint(glm::vec3(0.0f, -1.2f, 0.0f));

	left_lower_leg.set_translation_of_self_by_own_joint(glm::vec3(0.0f, 0.6f, 0.0f));	
	right_lower_leg.set_translation_of_self_by_own_joint(glm::vec3(0.0f, 0.6f, 0.0f));
	

	//setting up heirarchy graph 
	head.isHead = true;
	head.set_name("head");
	torso.set_parent(&head);
	torso.set_name("torso");
	head.set_scaling_factor(scale_head);

	//torso
	torso.set_scaling_factor(scale_torso);

	
	
	//Left and right of body
	
	torso.set_right(&legs);
	torso.set_left(&arms);
	torso.set_child(&head);

	arms.set_parent(&torso);
	legs.set_parent(&torso);
	arms.isOrganzingElement = true;
	legs.isOrganzingElement = true; 


	//arms 
	

	left_upper_arm.set_scaling_factor(scale_upper_arm);
	left_lower_arm.set_scaling_factor(scale_lower_arm);
	right_upper_arm.set_scaling_factor(scale_upper_arm);
	right_lower_arm.set_scaling_factor(scale_lower_arm);
	left_upper_arm.set_name("left upper arm");
	left_lower_arm.set_name("left lower arm");

	arms.set_left(&left_upper_arm);
	left_upper_arm.set_parent(&arms);
	left_upper_arm.set_left(&left_lower_arm);
	left_lower_arm.set_parent(&left_upper_arm);

	
	right_upper_arm.set_name("right upper arm");
	right_lower_arm.set_name("right lower arm");
	
	arms.set_right(&right_upper_arm);
	right_upper_arm.set_parent(&arms);
	right_upper_arm.set_right(&right_lower_arm);
	right_lower_arm.set_parent(&right_upper_arm);

	//legs 
	

	left_upper_leg.set_scaling_factor(scale_upper_leg);
	left_lower_leg.set_scaling_factor(scale_lower_leg);
	right_upper_leg.set_scaling_factor(scale_upper_leg);
	right_lower_leg.set_scaling_factor(scale_lower_leg);

	left_upper_leg.set_name("left upper leg");
	left_lower_leg.set_name("left lower leg");


	legs.set_left(&left_upper_leg);
	left_upper_leg.set_parent(&legs);
	left_upper_leg.set_left(&left_lower_leg);
	left_lower_leg.set_parent(&left_upper_arm);

	
	right_upper_leg.set_name("right upper leg");
	right_lower_leg.set_name("right lower leg");

	legs.set_right(&right_upper_leg);
	right_upper_leg.set_parent(&legs);
	right_upper_leg.set_right(&right_lower_leg);
	right_lower_leg.set_parent(&right_upper_leg);


	//getting order of elements for selection
	preorder(&torso, elements_in_order);

	//displaying elements in order
	// for(int i = 0; i < elements_in_order.size(); i++){
	// 	cout << elements_in_order[i]->get_name() << endl;
	// 	cout << elements_in_order[i] << endl;
	// }
	size_of_elements_vector = elements_in_order.size();
	selected_element = &unselect;

}

void Display()
{	
	
	//cout << endl;
	//cout << v.x << " vx " << v.y << " vy " << v.z << " vz " << endl << w.x << " wx "<< w.y << " wy " << w.z << " wz " << endl << u.x << " ux " << u.y << " uy " << u.z << " uz " << endl;
	//cout << endl;
	program.Bind();
	modelViewProjectionMatrix.loadIdentity();
	modelViewProjectionMatrix.pushMatrix();

	// Setting the view and Projection matrices
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	modelViewProjectionMatrix.Perspective(glm::radians(60.0f), float(width) / float(height), 0.1f, 100.0f);
	modelViewProjectionMatrix.LookAt(eye, center, global_up);

	torso.draw_self_and_children(&torso);
	modelViewProjectionMatrix.popMatrix();
	program.Unbind();
	
}

// Mouse callback function
void MouseCallback(GLFWwindow* lWindow, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && GLFW_PRESS == action)
		std::cout << "Mouse left button is pressed." << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	// cout << xoffset << endl;
	// cout << yoffset << endl;
	// cout << eye.z << endl;
	
	float new_eye_pos = eye.z + (-yoffset);
	if((new_eye_pos == 0) || (new_eye_pos < 0)){
		eye.z = 0;
	}
	else{
		eye.z += (-yoffset);
	}
	
	cout << eye.z << endl;
}

// Mouse position callback function
void CursorPositionCallback(GLFWwindow* lWindow, double xpos, double ypos)
{

	
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	int state2 = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	glm::vec3 a = eye - center;

	glm::vec3 normalized_up = glm::normalize(up);
	glm::vec3 normalized_eye = glm::normalize(eye);
	glm::vec3 a_normalized = glm::normalize(a);
	glm::vec3 right_numerator = glm::cross(up, a_normalized);
	float len = length(right_numerator);
	glm::vec3 right = right_numerator/len;
	
	glm::vec3 normalized_right = glm::normalize(right);
	if (state == GLFW_PRESS){
		if(first_use_of_mouse){
			previousXpos = xpos;
			previousYpos = ypos;
			first_use_of_mouse = false;
		}
		std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		double x_offset = xpos - previousXpos;
		double y_offset = previousYpos - ypos;
		// cout << previousXpos << " prev x" << endl;
		// cout << previousYpos << " prev y" << endl;
		// cout << xpos << " xpos" << endl;
		// cout << ypos << " ypos" << endl;
		previousXpos = xpos;
		previousYpos = ypos;
		// cout << x_offset << " x offset" << endl;
		
		// cout << y_offset << " y offset" << endl;
		double mouse_sensitivity = 0.009;
		x_offset *= mouse_sensitivity;
		y_offset *= 0.009;
		// eye.x += x_offset;
		// eye.y += y_offset;
		eye = a + center;
		
		glm::mat4 rotationy = modelViewProjectionMatrix.rotate(x_offset, global_up);
		glm::mat4 rotationx = modelViewProjectionMatrix.rotate(y_offset, right);

		glm::mat3 rotationy3 = glm::mat3(rotationy);
		glm::mat3 rotationx3 = glm::mat3(rotationx);
		
		glm::mat3 rotation = rotationx3 * rotationy3;
		glm::vec3 new_eye = (rotation*a) + center;
		
		eye = new_eye;

		a = eye - center;
		normalized_up = glm::normalize(up);
		normalized_eye = glm::normalize(eye);
		a_normalized = glm::normalize(a);
		right_numerator = glm::cross(up, a_normalized);
		len = length(right_numerator);
		right = right_numerator/len;
		normalized_right = glm::normalize(right);
		
		glm::vec3 new_up = glm::cross(glm::normalize(eye), glm::normalize(right));
		up = new_up;
		
		
	}
	if (state2 == GLFW_PRESS){
		if(first_use_of_mouse){
			previousXpos = xpos;
			previousYpos = ypos;
			first_use_of_mouse = false;
		}
		// std::cout << "Mouse position is: x - " << xpos << ", y - " << ypos << std::endl;
		double x_offset = xpos - previousXpos;
		double y_offset = previousYpos - ypos;
		// cout << previousXpos << " prev x" << endl;
		// cout << previousYpos << " prev y" << endl;
		// cout << xpos << " xpos" << endl;
		// cout << ypos << " ypos" << endl;
		previousXpos = xpos;
		previousYpos = ypos;
		// cout << x_offset << " x offset" << endl;
		double mouse_sensitivity = 0.009;
		x_offset *= mouse_sensitivity;
		y_offset *= mouse_sensitivity;
		//basic translation
		eye.x += x_offset;
		eye.y += y_offset;
		center.x += x_offset;
		center.y += y_offset;


	}
}


// Keyboard character callback function
void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	// for(int i = 0; i < elements_in_order.size(); i++){
	// 	cout << elements_in_order[i]->get_name() << endl;
	// }
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	// cout << size_of_elements_vector << endl;

	//selection of element to manipulate
	if((char)key == '.'){
		//forwards
		if(selector_index < (size_of_elements_vector - 1)){
			selector_index++;
		}
		// cout << selector_index << endl;
	}
	else if((char)key == ','){
		//backwards
		if(selector_index >= 0){
			selector_index--;
			
		}
		// cout << selector_index << endl;
	}
	if(selector_index > - 1){
		for(int i = 0; i < elements_in_order.size(); i++){
			elements_in_order[i]->isSelected = false;
		}
		selected_element = elements_in_order[selector_index];
		selected_element->isSelected = true;
	}
	if(selector_index == -1){
		for(int i = 0; i < elements_in_order.size(); i++){
			elements_in_order[i]->isSelected = false;
		}
		selected_element = &unselect;
		
	}

	//changing angles of selected elements 
	if((char)key == 'x'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.x -= 0.3f;
		selected_element->set_joint_angles(joint_angles);

	}
	else if((char)key == 'y'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.y -= 0.3f;
		selected_element->set_joint_angles(joint_angles);
	}
	else if((char)key == 'z'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.z -= 0.3f;
		selected_element->set_joint_angles(joint_angles);
	}
	else if((char)key == 'X'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.x += 0.3f;
		selected_element->set_joint_angles(joint_angles);
	}
	else if((char)key == 'Y'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.y += 0.3f;
		selected_element->set_joint_angles(joint_angles);
	}
	else if((char)key == 'Z'){
		glm::vec3 joint_angles = selected_element->get_joint_angles();
		joint_angles.z += 0.3f;
		selected_element->set_joint_angles(joint_angles);
	}
	
}


void CreateCube()
{
	// x, y, z, r, g, b, ...
	float cubeVerts[] = {
		// Face x-
		-1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face x+
		+1.0f,	+1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	+1.0f,	0.8f,	0.2f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.8f,	0.2f,	0.2f,
		// Face y-
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face y+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.8f,	0.2f,
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.8f,	0.2f,
		// Face z-
		+1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	-1.0f,	0.2f,	0.2f,	0.8f,
		// Face z+
		+1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		+1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	+1.0f,	+1.0f,	0.2f,	0.2f,	0.8f,
		-1.0f,	-1.0f,	+1.0f,	0.2f,	0.2f,	0.8f
	};

	GLuint vertBufferID;
	glGenBuffers(1, &vertBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerts), cubeVerts, GL_STATIC_DRAW);
	GLint posID = glGetAttribLocation(program.GetPID(), "position");
	glEnableVertexAttribArray(posID);
	glVertexAttribPointer(posID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	GLint colID = glGetAttribLocation(program.GetPID(), "color");
	glEnableVertexAttribArray(colID);
	glVertexAttribPointer(colID, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

}

void FrameBufferSizeCallback(GLFWwindow* lWindow, int width, int height)
{
	glViewport(0, 0, width, height);
}

void Init()
{
	glfwInit();
	ConstructRobot();
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment2 - Ivan Martinez", NULL, NULL);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetMouseButtonCallback(window, MouseCallback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetCharCallback(window, CharacterCallback);	
	glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	// selector(pressed_key, &torso);
	program.SetShadersFileName(vertShaderPath, fragShaderPath);
	program.Init();

	CreateCube();
	
	
}


int main()
{	
	Init();
	//display pointers and data in order 
	// cout << " start " << endl;
	// for(int i = 0; i < elements_in_order.size(); i++){
	// 	cout << elements_in_order[i]->get_name() << endl;
	// 	// cout << elements_in_order[i] << endl;
	// }
	// cout << " end " << endl;
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