#ifndef INTERSECT_H
#define INTERSECT_H

#include "Loader.h"

#include <vector>
#include <glm/gtx/fast_square_root.hpp>

class Intersections{
    private:
    glm::vec3 PhongLighting(Light, glm::vec3, glm::vec3, glm::vec3, Ray, bool);
    glm::vec4 intersect_Sphere(Sphere, Light, Ray);
    glm::vec4 intersect_Plane(Plane, Light, Ray);
    glm::vec4 intersect_Triangle(Triangle, Light, Ray);

    glm::vec4 traceColour(Loader,Ray);
    glm::vec4 traceReflection(glm::vec4,Loader, Ray, int);
    glm::vec4 traceShadow(glm::vec4, Loader,Ray);

    glm::vec4 blendColour(glm::vec4, glm::vec4);
    glm::vec3 normalTmp;
    glm::vec3 normalVec;
    float reflectionType;
    public:
    glm::vec4 getColour(Loader,Ray);
};

#endif