#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Ray
{
public:
	// Ray();
	// ~Ray();

// private:
	glm::vec3 origin;
	glm::vec3 direction;

    Ray(glm::vec3 ori, glm::vec3 dir){
        origin = ori;
        direction = dir;
    }

};