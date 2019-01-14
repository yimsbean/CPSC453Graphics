
#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include <glm/glm.hpp>

class Ray{
    public:
    glm::vec3 origin;
    glm::vec3 direction;
};

class Light{
    public:
    glm::vec3 point;
    float strength;
};

class SceneObjects{};
class Sphere : public SceneObjects{
    public:
    glm::vec3 centre;
    float radius;

    glm::vec3 colour;
    float type;         //simple(0), reflective(1), metallic(2)
};
class Plane : public SceneObjects{
    public:
    glm::vec3 normal;
    glm::vec3 point;

    glm::vec3 colour;
    float type;         //simple(0), reflective(0)
};
class Triangle : public SceneObjects{
    public:
    glm::vec3 C0;
    glm::vec3 C1;
    glm::vec3 C2;
    
    glm::vec3 colour;
    float type;         //simple(0), reflective(1)
};

#endif