/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"

#include <iostream>
#include <string>

 //**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

int currentScene;
int level;

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
	currentScene = 0;
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
	int width = 512;
	int height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}


	glfwSetWindowUserPointer(window, this);	//find a way to send informatino to key callback


	//Set the custom function that tracks key presses
	glfwSetKeyCallback(window, KeyCallback);

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

	std::cout << "OpenGL [ " << version << " ] " << "with GLSL [ " << glslver << " ] " << "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	else if (key >= GLFW_KEY_1 && key <= GLFW_KEY_7 && action == GLFW_PRESS) {
		currentScene = key % GLFW_KEY_1;
		level = 0;
		switchScenes(window);
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		currentScene = (currentScene + 6) % 7;
		level = 0;
		switchScenes(window);
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		currentScene = ++currentScene % 7;
		level = 0;
		switchScenes(window);
	}
	else if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		int lastLevel = level;
		//Sierpinski Triangle needs at least 20 iterations
		//MengerSponge has a known major performance issue after 5 iterations
		//Others should be fine with less than 7
		if (currentScene == 3) {
			level = ++level < 5 ? level : 5;
		}
		else if(currentScene == 4 || currentScene == 5) {
			level=++level < 20 ? level : 20;
		}
		else {
			level=++level < 7 ? level : 7;
		}
		//Avoid redrawing the same scene to prevent potential memory leak
		if (lastLevel != level) {
			switchScenes(window);
		}
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		level = --level > 0 ? level : 0;
		switchScenes(window);
	}
}
void switchScenes(GLFWwindow* window) {
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	switch (currentScene) {
	case 0: program->getScene()->changeTo_SquaresAndDiamons(level); break;
	case 1: program->getScene()->changeTo_ParametricSpiral(level); break;
	case 2: program->getScene()->changeTo_SierpinskiTriangle(level); break;
	case 3: program->getScene()->changeTo_MengerSponge(level); break;
	case 4: program->getScene()->changeTo_SierpinskiTriangleReloaded(level); break;
	case 5: program->getScene()->changeTo_BarnsleyFern(level); break;
	case 6: program->getScene()->changeTo_HilbertSpaceFillingCurve(level); break;
	}
}
