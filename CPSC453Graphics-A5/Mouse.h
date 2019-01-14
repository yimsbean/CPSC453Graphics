#ifndef MOUSE_H_
#define MOUSE_H_

#include <vector>

#include "Geometry.h"
#include "Node.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Mouse {
public:
    static bool isPressed;

    static double xLast;
    static double yLast;

    static int scrollValue;
};

#endif /* MOUSE_H_ */
