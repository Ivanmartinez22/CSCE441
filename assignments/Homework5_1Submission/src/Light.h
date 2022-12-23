#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light
{
public:
	Light();
	~Light();

// private:
	glm::vec3 position;
	glm::vec3 color;

};
