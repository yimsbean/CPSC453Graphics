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

void mouse_button_callback(GLFWwindow* , int , int , int );
void mouse_motion_callback(GLFWwindow* , double , double);
void mouse_scroll_callback(GLFWwindow* , double, double);
int part = 0;

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
	scene = new Scene(window, renderingEngine);

	//Main render loop
	while(!glfwWindowShouldClose(window)) {
		glfwMakeContextCurrent(NULL);
		scene->displayGaussianScene();
		glfwMakeContextCurrent(window);
		
		if(part == 1)
			scene->displayBackScene();
		scene->displayScene();
		glfwSwapBuffers(window);
		glfwWaitEvents();
		glfwSwapInterval(1);
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
	int width = 512;
	int height = 512;
	window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
	if (!window) {
		std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
		glfwTerminate();
		return;
	}
	glfwSetWindowUserPointer(window, this);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, mouse_motion_callback);
	glfwSetScrollCallback(window, mouse_scroll_callback);
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

//int part = 0;
const char* backgrounds[3] = {
	"images/background1-asphalt.jpg",
	"images/background2-marble.jpg",
	"images/background3-wood.jpg"
};
int backNo=0;
const char* imageList[6] = {
	"images/image1-mandrill.png",
	"images/image2-uclogo.png",
	"images/image3-aerial.jpg",
	"images/image4-thirsk.jpg",
	"images/image5-pattern.png",
	"images/image6-bubble.png"
};
int imageNo=0;
float factor=1.0f;
float radians=0.0f;

int Global::sepia = 0;
int Global::gray1 = 0;
int Global::gray2 = 0;
int Global::gray3 = 0;
int Global::vert = 0;
int Global::hori = 0;
int Global::unsharp = 0;
int Global::background = 0;
GLfloat colors[][3] = { { 0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f } };


class Point{
public:
	double x;
	double y;
	Point(double a, double b){x = a; y = b;};
};
Point origin=Point(0,0);
Point mouseLocation = Point(0,0);
bool dragMutex = false;
bool initialClick = true;

int gaussianKernelSize=1;


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if(action == GLFW_PRESS){
            dragMutex = true;
			if(initialClick){
				glfwGetCursorPos(window, &origin.x, &origin.y);
				initialClick = false;
			}
		}
        else if(action == GLFW_RELEASE){
            dragMutex = false;
			initialClick = true;
		}
    }
}
void mouse_motion_callback(GLFWwindow* window, double x, double y){
	if(dragMutex) {
        glfwGetCursorPos(window, &mouseLocation.x, &mouseLocation.y);
		changeImage(window);
    }
}
void mouse_scroll_callback(GLFWwindow* window, double x, double y){
	if(y > 0)
		factor = factor >= 10.0f ? factor : factor + y*0.25f ;
	else
		factor = factor <= 0.25f ? factor : factor + y*0.25f ;
	if(x > 0)
		radians = radians + M_PI/6 >= 2*M_PI ? radians : radians + x*M_PI/6 ;
	else
		radians = radians - M_PI/6 <= -2*M_PI ? radians : radians + x*M_PI/6 ;
	changeImage(window);
}


void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	//Key codes are often prefixed with GLFW_KEY_ and can be found on the GLFW website
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	//Keyboard Only
	if(action == GLFW_PRESS){
		if(key == GLFW_KEY_1) { part = 0; 		glDisable(GL_BLEND);}
		else if(key == GLFW_KEY_2){ part = 1;	glDisable(GL_BLEND);}
		else if(key == GLFW_KEY_3){ part = 2;	glDisable(GL_BLEND);}
		//Part 1
		// PgUp (Prev Image), PgDn (Next Image), selecting Images
		if (key == GLFW_KEY_PAGE_UP) {
			imageNo = ++imageNo % 6;
			factor = 1.0f; radians = 0.0f; origin = mouseLocation = Point(0,0); backNo = 0; gaussianKernelSize = 1;
		}else if(key == GLFW_KEY_PAGE_DOWN){
			imageNo = (imageNo+5) % 6; 
			factor = 1.0f; radians = 0.0f; origin = mouseLocation = Point(0,0); backNo = 0; gaussianKernelSize = 1;
		}
		// W(larger), W(smaller), Scaling Images
		if (key == GLFW_KEY_W) {
			factor = factor >= 10.0f ? factor : factor + 0.25f ;
		}else if(key == GLFW_KEY_S){
			factor = factor <= 0.25f ? factor : factor - 0.25f ;
		}
		// A(Rotate Left), D(Rotate Right), Rotating Images
		if (key == GLFW_KEY_A) {
			radians = radians + M_PI/6 >= 2*M_PI ? radians : radians + M_PI/6 ;
		}else if(key == GLFW_KEY_D){
			radians = radians - M_PI/6 <= -2*M_PI ? radians : radians - M_PI/6 ;
		}
		// < (Left), >(Right), ^(Up), v(Down) Panning Images
		if(key == GLFW_KEY_LEFT){
			mouseLocation.x -= 75;
		}else if(key == GLFW_KEY_RIGHT){
			mouseLocation.x += 75;
		}
		if (key == GLFW_KEY_UP) {
			mouseLocation.y -= 75;
		}else if(key == GLFW_KEY_DOWN){
			mouseLocation.y += 75;
		}

		if(key == GLFW_KEY_G){
			Global::gray1 ^= 1;
		}
		else if(key == GLFW_KEY_H){
			Global::gray2 ^= 1;
		}
		else if(key == GLFW_KEY_J){
			Global::gray3 ^= 1;
		}
		else if(key == GLFW_KEY_K){
			Global::sepia ^= 1;
		}
		else if(key == GLFW_KEY_B){
			Global::vert ^= 1;
		}
		else if(key == GLFW_KEY_N){
			Global::hori ^= 1;
		}
		else if(key == GLFW_KEY_M){
			Global::unsharp ^= 1;
		}
	
		//Part 4 Gaussian Blurr
		if (key == GLFW_KEY_HOME) {
			gaussianKernelSize = gaussianKernelSize + 2 > 21 ? gaussianKernelSize: gaussianKernelSize + 2;
		}else if(key == GLFW_KEY_END){
			gaussianKernelSize = gaussianKernelSize - 2 < 1 ? 1: gaussianKernelSize - 2;
		}
	
		//Part 5 Alpha blending
		if (key == GLFW_KEY_LEFT_BRACKET) {
			backNo = ++backNo % 3;
		}else if(key == GLFW_KEY_RIGHT_BRACKET){
			backNo = (backNo+2) % 3;
		}

		//Bonus Part, Gaussian Kernel
		if(part == 2){
			if (key == GLFW_KEY_HOME) {
				gaussianKernelSize = gaussianKernelSize*3 > 1000 ? gaussianKernelSize : gaussianKernelSize*3;
			}else if(key == GLFW_KEY_END){
				gaussianKernelSize = gaussianKernelSize/3 < 1 ? 1 : ((gaussianKernelSize/3)%2==0? gaussianKernelSize/3 + 1:gaussianKernelSize/3);
			}
		}


		changeImage(window);
	}
}
void changeImage(GLFWwindow* window){
	Global::background = 0;
	Program* program = (Program*)glfwGetWindowUserPointer(window);
	float xdiff = (origin.x-mouseLocation.x);
	float ydiff = (origin.y-mouseLocation.y);
	if(part == 0 || part == 2){
		program->getScene()->switchImages(window, imageList[imageNo] ,xdiff,ydiff,factor, radians, gaussianKernelSize);
	}
	else if(part == 1){
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		Global::background = 1;
		program->getScene()->switchImagesWithBackground	(backgrounds[backNo]);
		program->getScene()->switchImages	(window, imageList[imageNo],xdiff,ydiff, factor, radians, gaussianKernelSize);
	}
}