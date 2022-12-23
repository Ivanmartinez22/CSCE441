#version 120

attribute vec3 vPositionModel; // in object space
attribute vec3 vNormalModel; // in object space
uniform vec3 eye; 


uniform mat4 model;
uniform mat4 transformed_model;
uniform mat4 view;
uniform mat4 projection;







varying vec3 color;

vec4 worldPositionModel = model * vec4(vPositionModel, 1.0) ;



vec4 worldNormalModel = transformed_model * vec4(vNormalModel, 1.0);



void main()
{
	
	
	vec3 black = vec3(0.0, 0.0, 0.0);
	gl_Position = projection * view * model * vec4(vPositionModel, 1.0);
	vec3 eye_ = eye - vec3(worldPositionModel) ;
	eye_ = normalize(eye_);
	vec3 normal = normalize(vec3(worldNormalModel));

	float n_e = dot(normal, eye_);
	float angle = acos(n_e);
	angle = (angle * 180)/3.1459;
	//cout << angle << endl;
	if((angle >= 75) && (angle <= 110)){
		black = vec3(1.0, 1.0, 1.0);
	}

	// for(int i = 0; i < NUM_LIGHTS; i++){
	// 	vec3 light = lights[i].position - vec3(worldPositionModel);
	// 	light = normalize(light);
	// 	vec3 eye_ = eye - vec3(worldPositionModel) ;
	// 	eye_ = normalize(eye_);
		
	// 	vec3 reflection = (2 * (dot(light, vec3(worldNormalModel))) * (vec3(worldNormalModel))) - light;
	// 	float L_N = dot(light, vec3(worldNormalModel));
	// 	float R_E = dot(reflection, eye_);


	// 	I += (lights[i].color * ((kd * max(0, L_N)) + (ks * pow(max(0, R_E), s)))); 
	// }

	color = black;
}
