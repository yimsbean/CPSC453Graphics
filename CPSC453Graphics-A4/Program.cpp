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


Program::Program() {
	setupWindow();
}

Program::~Program() {
}

void Program::start() {
	loadScene1();
	reload();
	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		draw();
		image.Render();
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

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if(action == GLFW_PRESS){
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}

		if (key == GLFW_KEY_UP){					//pitch up
			Camera::pitch -= 0.3;
		}else if (key == GLFW_KEY_DOWN){			//pitdh down
			Camera::pitch += 0.3;
		}
		if (key == GLFW_KEY_LEFT){					//yaw Left
			Camera::yaw += 0.3;
		}else if (key == GLFW_KEY_RIGHT){			//yaw Right
			Camera::yaw -= 0.3;
		}
		if (key == GLFW_KEY_LEFT_SHIFT){			//Zoom in
			Camera::back += 0.3;
		}else if (key == GLFW_KEY_LEFT_CONTROL){	//Zoom out
			Camera::back -= 0.3;
		}

		if (key == GLFW_KEY_A){					//left
			Camera::CameraLocation_X += 0.3;
		}else if (key == GLFW_KEY_D){			//right
			Camera::CameraLocation_X -= 0.3;
		}
		if (key == GLFW_KEY_R){					//front
			Camera::CameraLocation_Y += 0.3;
		}else if (key == GLFW_KEY_F){			//back
			Camera::CameraLocation_Y -= 0.3;
		}
		if (key == GLFW_KEY_W){					//Up
			Camera::CameraLocation_Z -= 0.3;
		}else if (key == GLFW_KEY_S){			//Down
			Camera::CameraLocation_Z += 0.3;
		}

		if (key == GLFW_KEY_LEFT_BRACKET){					//Pitch Up
			Camera::FOV = Camera::FOV - M_PI / 12 < 0 ? Camera::FOV : Camera::FOV - M_PI / 12;
		}else if (key == GLFW_KEY_RIGHT_BRACKET){			//Pitch Down
			Camera::FOV = Camera::FOV + M_PI / 12 > 2*M_PI ? Camera::FOV : Camera::FOV + M_PI / 12;
		}
		
		Program * program = (Program*)glfwGetWindowUserPointer(window);

		if (key == GLFW_KEY_1){				
			program->loadScene1();
		}else if (key == GLFW_KEY_2){
			program->loadScene2();
		}else if (key == GLFW_KEY_3){
			program->loadScene3();
		}
		program->reload();
	}
}

void Program::draw(){
	Ray r;
	r.origin = glm::vec3(Camera::yaw, Camera::pitch, Camera::back);
	glm::vec4 temp;
	for (unsigned int i = 0; i < image.Width(); i++) {
		for (unsigned int j = 0; j < image.Height(); j++) {
			// set ray's direction
			r.direction = glm::fastNormalize(
				glm::vec3(
					-1.0 + (2.0*i)/image.Width(),	// -1 ~ 1
					-1.0 + (2.0*j)/image.Height(),// -1 ~ 1
					-(1.0/tan(Camera::FOV/2.0))
				)-r.origin
			);
			temp = intersect.getColour(load,r);
			if(temp.w < 2048)
				image.SetPixel(i, j, temp);
		}
	}
}
void Program::initialize(){
	Camera::CameraLocation_X = Camera::CameraLocation_Y = Camera::CameraLocation_Z = 0;
	Camera::yaw = Camera::pitch = Camera::back = Camera::roll = 0;
	Camera::FOV = (M_PI/4);
}
void Program::loadScene1(){
	load.initialize();
	load.readFromSceneFile("scenes/scene1.txt");
	initialize();
}
void Program::loadScene2(){
	load.initialize();
	load.readFromSceneFile("scenes/scene2.txt");
	initialize();
}
void Program::loadScene3(){
	load.initialize();
	load.readFromSceneFile("scenes/scene3.txt");
	initialize();
}
void Program::reload(){
	image.Initialize();
	
	for(int i = 0; i < load.light.size() ; ++i){
		Light l = load.light.at(i);
		load.light.at(i).point = glm::vec3(
			l.point.x + Camera::CameraLocation_X, 
			l.point.y + Camera::CameraLocation_Y, 
			l.point.z + Camera::CameraLocation_Z
		);
	}
	for(int i = 0; i < load.sphere.size() ; ++i){
		Sphere s= load.sphere.at(i);
		load.sphere.at(i).centre = glm::vec3(
			s.centre.x + Camera::CameraLocation_X, 
			s.centre.y + Camera::CameraLocation_Y, 
			s.centre.z + Camera::CameraLocation_Z
		);
	}
	for(int i = 0; i < load.plane.size() ; ++i){
		Plane p = load.plane.at(i);
		load.plane.at(i).point = glm::vec3(
			p.point.x + Camera::CameraLocation_X, 
			p.point.y + Camera::CameraLocation_Y, 
			p.point.z + Camera::CameraLocation_Z
		);
	}
	for(int i = 0; i < load.triangle.size() ; ++i){
		Triangle t= load.triangle.at(i);
		load.triangle.at(i).C0 = glm::vec3(
			t.C0.x + Camera::CameraLocation_X,
			t.C0.y + Camera::CameraLocation_Y, 
			t.C0.z + Camera::CameraLocation_Z);
		load.triangle.at(i).C1 = glm::vec3(
			t.C1.x + Camera::CameraLocation_X, 
			t.C1.y + Camera::CameraLocation_Y, 
			t.C1.z + Camera::CameraLocation_Z);
		load.triangle.at(i).C2 = glm::vec3(
			t.C2.x + Camera::CameraLocation_X, 
			t.C2.y + Camera::CameraLocation_Y, 
			t.C2.z + Camera::CameraLocation_Z);
	}
	draw();
	image.Render();
}