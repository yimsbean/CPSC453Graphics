#ifndef LOADER_H
#define LOADER_H

#include <vector>
#include "SceneObjects.h"
class Loader{
    public:
    void initialize();
    void readFromSceneFile(const char*);

    std::vector<Light> light;
    std::vector<Sphere> sphere;
    std::vector<Plane> plane;
    std::vector<Triangle> triangle;
};

#endif