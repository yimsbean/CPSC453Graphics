#ifndef SPHERE_H_
#define SPHERE_H_

#include "Geometry.h"
#include <string>

class Sphere : public Geometry {

public:
    Sphere(std::string sourcePath, std::string imagePath);

    virtual ~Sphere();

    void setOrientation(glm::mat4 orientation);

private:
    glm::mat4 orientation;
};

#endif /* SPHERE_H_ */

