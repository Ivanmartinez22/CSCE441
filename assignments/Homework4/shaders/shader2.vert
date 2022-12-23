#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space
uniform vec3 eye; 


uniform mat4 model;
uniform mat4 transformed_model;
uniform mat4 view;
uniform mat4 projection;


varying vec3 vPositionModelvary;
varying vec3 vNormalModelvary;



void main()
{
	
	
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	vPositionModelvary = vPositionModel;
	vNormalModelvary = vNormalModel;
	

	
}
