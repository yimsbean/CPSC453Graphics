/*
 * Program.h
 *  Base class of a rendering program. Has a scene to render, a rendering engine and a window to render to.
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */


#ifndef PROGRAM_H_
#define PROGRAM_H_

//std
#include <iostream>
#include <string>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//**Must include glad and GLFW in this order or it breaks**
#include "glad/glad.h"
#include <GLFW/glfw3.h>

// custom
#include "Camera.h"
#include "Mouse.h"
#include "RenderingEngine.h"
#include "Scene.h"


//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
struct GLFWwindow;
class RenderingEngine;
class Scene;

class Program {
public:
    Program();
    virtual ~Program();

    //Creates the rendering engine and the scene and does the main draw loop
    void start();

    //Initializes GLFW and creates the window
    void setupWindow();

    //Prints system specs to the console
    void QueryGLVersion();
    Scene* getScene(){return scene;};
private:
    GLFWwindow* window;
    RenderingEngine* renderingEngine;
    Scene* scene;
};

//Functions passed to GLFW to handle errors and keyboard input
//Note, GLFW requires them to not be member functions of a class
void ErrorCallback(int error, const char* description);
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#endif /* PROGRAM_H_ */
