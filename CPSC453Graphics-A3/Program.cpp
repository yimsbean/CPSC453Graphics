/*
 * Program.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "Program.h"
#include "Data.h"

#include <iostream>
#include <string>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "RenderingEngine.h"
#include "Scene.h"

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
	while(!glfwWindowShouldClose(window)) {
		scene->displayScene();
		glfwSwapBuffers(window);
		if(Data::scEnabled)
			glfwPollEvents();
		else{
			glfwWaitEvents();
			glfwWaitEventsTimeout(1);
		}
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
	glfwWindowHint(GLFW_SAMPLES, 8);
	int width = 1024;
	int height = 1024;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}
	glfwSetWindowUserPointer(window, this);
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

	std::cout << "OpenGL [ " << version << " ] "
		<< "with GLSL [ " << glslver << " ] "
		<< "on renderer [ " << renderer << " ]" << std::endl;
}

void ErrorCallback(int error, const char* description) {
	std::cout << "GLFW ERROR " << error << ":" << std::endl;
	std::cout << description << std::endl;
}

int sceneNo = 0; //Q1 (2 Scene), Q2 (3 Scene), Q3 (3 Scene)
//#include <stdio.h>

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if(action == GLFW_PRESS && key == GLFW_KEY_ESCAPE){
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if(action == GLFW_PRESS){
		if (key == GLFW_KEY_LEFT) {
			switch(Data::QuestionNumber){
				case 0: 		 sceneNo = (sceneNo + 3) % 2; break;
				case 1:  case 2: sceneNo = (sceneNo + 4) % 3; break;
				default: 		 sceneNo = 0;
			}
		}
		else if (key == GLFW_KEY_RIGHT) {
			switch(Data::QuestionNumber){
				case 0:  		 sceneNo = ++sceneNo % 2; break;
				case 1: case 2:  sceneNo = ++sceneNo % 3; break;
				default:		 sceneNo = 0;
			}
		}
		if (key == GLFW_KEY_UP) {
			Data::scRate = Data::scRate + 0.5 > 10 ? Data::scRate : Data::scRate+0.5; return;
		}else if (key == GLFW_KEY_DOWN) {
			Data::scRate = Data::scRate - 0.5 < -10 ? Data::scRate : Data::scRate-0.5; return;
		}
		if (key == GLFW_KEY_1){ 	 Data::QuestionNumber = 0;  sceneNo = 0; Data::scRate = 1; }
		else if (key == GLFW_KEY_2){ Data::QuestionNumber = 1;  sceneNo = 0; Data::scRate = 1; }
		else if (key == GLFW_KEY_3){ Data::QuestionNumber = 2;  sceneNo = 0; Data::scRate = 1; }
		else if (key == GLFW_KEY_4){ Data::QuestionNumber = 3;  sceneNo = 0; Data::scRate = 1; }
		switchScenes(window);
	}
}

void switchScenes(GLFWwindow* window) {
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	switch (Data::QuestionNumber) {
		case 0: 
			switch(sceneNo){
				case 0: program->getScene()->Q1_Scene1(); break;
				case 1: program->getScene()->Q1_Scene2(); break;
			}; 
			break;
		case 1: 
			switch(sceneNo){
				case 0: program->getScene()->Q2_Font1();  break;
				case 1: program->getScene()->Q2_Font2();  break;
				case 2: program->getScene()->Q2_Font3();  break;
			}; break;
		case 2: 
			switch(sceneNo){
				case 0: program->getScene()->Q3_Scroll1();  break;
				case 1: program->getScene()->Q3_Scroll2();  break;
				case 2: program->getScene()->Q3_Scroll3();  break;
			}; break;
		case 3: 
			program->getScene()->Bonus1();  break;
	}
}