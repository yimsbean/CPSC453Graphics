#ifndef CAMERA_H_
#define CAMERA_H_

#include "Math.h"

#include <glm/glm.hpp>
//**Must include glad and GLFW in this order or it breaks**
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
//
class Camera {
public:
    static const int FOV_DEFAULT = 90;

    static int MIN_ZOOM;
    static const int MAX_ZOOM = 200;
    static const int DEFAULT_ZOOM = 30;
    static const int ZOOM_DIVISOR = 1;

    static float PHI_DEFAULT;
    static float THETA_DEFAULT;

    static float φ;
    static float θ;
    static float zNear;
    static float zFar;
    static float fieldOfView;
    static float aspectRatio;
    static float zoom;

    static glm::vec3 baseLocation;
    static glm::vec3 modifiedLocation;
    static glm::vec3 direction;
    static glm::vec3 up;
};

#endif
