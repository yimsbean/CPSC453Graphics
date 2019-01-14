/*
* Program.cpp
*
*  Created on: Sep 10, 2018
*      Author: John Hall
*/

#include "Program.h"



Program::Program() {
    setupWindow();
}

Program::~Program() {
    //Must be cleaned up in the destructor because these are allocated to the heap
    delete renderingEngine;
    delete scene;
}

void Program::start() {
    renderingEngine = new RenderingEngine();
    scene = new Scene(renderingEngine);
    //Main render loop
    while (!glfwWindowShouldClose(window)) {
        scene->displayScene();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

}

void Program::setupWindow() {
    //Initialize the GLFW windowing system
    if (!glfwInit()) {
        std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
        return;
    }

    //Set the custom error callback function
    //Errors will be printed to the console
    glfwSetErrorCallback(ErrorCallback);

    //Attempt to create a window with an OpenGL 4.1 core profile context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(Scene::WIDTH, Scene::HEIGHT, "CPSC 453 OpenGL Boilerplate", 0, 0);
    if (!window) {
        std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
        glfwTerminate();
        return;
    }
    glfwSetWindowUserPointer(window, this);
    //Set the custom function that tracks key presses
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);
    glfwSetCursorPosCallback(window, CursorPositionCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    //Bring the new window to the foreground (not strictly necessary but convenient)
    glfwMakeContextCurrent(window);

    //Intialize GLAD (finds appropriate OpenGL configuration for your system)
    if (!gladLoadGL()) {
        std::cout << "GLAD init failed" << std::endl;
        return;
    }

    //Query and print out information about our OpenGL environment
    QueryGLVersion();
}



void Program::QueryGLVersion() {
    // query opengl version and renderer information
    std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    std::cout << "OpenGL [ " << version << " ] "
        << "with GLSL [ " << glslver << " ] "
        << "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
    std::cout << "GLFW ERROR " << error << ":" << std::endl;
    std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Program* program = (Program*)glfwGetWindowUserPointer(window);
    //Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GL_TRUE); break;
            case GLFW_KEY_H:
            case GLFW_KEY_LEFT:
                program->getScene()->slowDown();    break;
            case GLFW_KEY_L:
            case GLFW_KEY_RIGHT:
                program->getScene()->speedUp();     break;
            case GLFW_KEY_P:
                program->getScene()->togglePause(); break;
            case GLFW_KEY_S:
                program->getScene()->resetScene();          break;
            case GLFW_KEY_C:
                program->getScene()->initializeCamera();    break;
            case GLFW_KEY_GRAVE_ACCENT:
                program->getScene()->changePlanet(0);       break;
            case GLFW_KEY_1:
                program->getScene()->changePlanet(1);       break;
            case GLFW_KEY_2:
                program->getScene()->changePlanet(2);       break;
            case GLFW_KEY_3:
                program->getScene()->changePlanet(3);       break;
            case GLFW_KEY_4:
                program->getScene()->changePlanet(4);       break;
            case GLFW_KEY_5:
                program->getScene()->changePlanet(5);       break;
            case GLFW_KEY_6:
                program->getScene()->changePlanet(6);       break;
            case GLFW_KEY_7:
                program->getScene()->changePlanet(7);       break;
            case GLFW_KEY_8:
                program->getScene()->changePlanet(8);       break;
            case GLFW_KEY_9:
                program->getScene()->changePlanet(9);       break;
            case GLFW_KEY_0:
                program->getScene()->changePlanet(10);       break;
            case GLFW_KEY_MINUS:
                program->getScene()->changePlanet(11);       break;
            case GLFW_KEY_EQUAL:
                program->getScene()->changePlanet(12);       break;
            case GLFW_KEY_BACKSPACE:
                program->getScene()->changePlanet(13);       break;
        }
    }
}

/**
 * Scrolling zooms in and out
 */
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    Camera::zoom -= yOffset/Camera::ZOOM_DIVISOR;
    if (Camera::zoom < Camera::MIN_ZOOM) {
        Camera::zoom = Camera::MIN_ZOOM;
    } else if (Camera::zoom > Camera::MAX_ZOOM) {
        Camera::zoom = Camera::MAX_ZOOM;
    }
}

/**
 * Checks if mouse is pressed or not.
 * If mouse is pressed, record mouse coordinates.
 */
void MouseCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        switch (action) {
            case GLFW_PRESS:
                Mouse::isPressed = true;
                break;
            case GLFW_RELEASE:
                Mouse::isPressed = false;
                break;
        }
    }
}

/**
 * Cursor position for dragging.
 */
void CursorPositionCallback(GLFWwindow *window, double xpos, double ypos) {
    if (Mouse::isPressed) {
        Camera::φ += Math::degreesToRadians((Mouse::xLast - xpos) / Mouse::scrollValue);
        Camera::θ += Math::degreesToRadians((Mouse::yLast - ypos)/ Mouse::scrollValue);
    }
    Mouse::xLast = xpos;
    Mouse::yLast = ypos;
}
