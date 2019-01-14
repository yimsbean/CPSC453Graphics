#ifndef NODE_H_
#define NODE_H_

#include <vector>

#include "Geometry.h"
#include "texture.h"

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #define _USE_MATH_DEFINES
// #include <math.h>

class Node {
public:
    // TODO clean
    std::vector<Geometry> geometry;
    MyTexture texture;
    // Node* orbit; // TODO unused

    float radius;
    int level = 36;
    float rotation;
    float rotationSpeed;
    float rotationSpeedBase;

    float revolution;
    float revolutionSpeed;
    float revolutionSpeedBase;

    glm::vec3 distance;
    glm::vec3 position;

    glm::mat4 axis;
    glm::mat4 rotate;
    glm::mat4 transform;
    glm::mat4 translate;
};

#endif /* NODE_H_ */
