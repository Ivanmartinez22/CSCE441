#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space
uniform vec3 eye; 


uniform mat4 model;
uniform mat4 transformed_model;
uniform mat4 view;
uniform mat4 projection;

struct lightStruct
{
	vec3 position;
	vec3 color;
};

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

#define NUM_LIGHTS 2

uniform lightStruct lights[NUM_LIGHTS];



uniform vec3 ka;
uniform vec3 kd;
uniform vec3 ks;
uniform float s;

varying vec3 color;

vec4 worldPositionModel = model * vec4(vPositionModel, 1.0) ;



vec4 worldNormalModel = transformed_model * vec4(vNormalModel, 1.0);



void main()
{
	
	
	vec3 I = ka;
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	for(int i = 0; i < NUM_LIGHTS; i++){
		vec3 light = lights[i].position - vec3(worldPositionModel);
		light = normalize(light);
		vec3 eye_ = eye - vec3(worldPositionModel) ;
		eye_ = normalize(eye_);
		
		vec3 reflection = (2 * (dot(light, vec3(worldNormalModel))) * (vec3(worldNormalModel))) - light;
		float L_N = dot(light, vec3(worldNormalModel));
		float R_E = dot(reflection, eye_);


		I += (lights[i].color * ((kd * max(0, L_N)) + (ks * pow(max(0, R_E), s)))); 
	}

	color = vec3(I.x, I.y, I.z);
}
