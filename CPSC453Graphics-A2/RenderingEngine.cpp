/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"
#include <iostream>
//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"
#include "Program.h"

RenderingEngine::RenderingEngine() {
	shaderProgram = ShaderTools::InitializeShaders();
	shaderProgram2 = ShaderTools::InitializeShaders();
	shaderProgram3 = ShaderTools::InitializeShaders();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}
	if (shaderProgram2 == 0) {
		std::cout << "Program could not initialize shaders2, TERMINATING" << std::endl;
		return;
	}
	if (shaderProgram3 == 0) {
		std::cout << "Program could not initialize shaders2, TERMINATING" << std::endl;
		return;
	}
}

RenderingEngine::~RenderingEngine() {

}
void RenderingEngine::RenderScene(const std::vector<Geometry>& objects,int steps,GLuint framebuffer, GLuint shader) {

	if(steps == 1){			//background
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}else if(steps == 2){	//gaussianBuffer(to Framebuffer)
		
	}else{					//foreground
		if(Global::background == 0 ){
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}
	// bind our shader program and the vertex array object containing our
	// scene geometry, then tell OpenGL to draw our geometry
	glUseProgram(shader);
	
	for (const Geometry& g : objects) {
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode, 0, g.verts.size());
		// reset state to default (no shader or geometry bound)
		glBindVertexArray(0);
	}
	glUseProgram(0);
	// check for an report any OpenGL errors
	CheckGLErrors();
}
void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);

	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
}

void RenderingEngine::deleteBufferData(Geometry& geometry) {
	glDeleteBuffers(1, &geometry.vertexBuffer);
	glDeleteBuffers(1, &geometry.normalBuffer);
	glDeleteBuffers(1, &geometry.colorBuffer);
	glDeleteBuffers(1, &geometry.uvBuffer);
	glDeleteVertexArrays(1, &geometry.vao);
}

bool RenderingEngine::CheckGLErrors() {
	bool error = false;
	for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
	{
		std::cout << "OpenGL ERROR:  ";
		switch (flag) {
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl; break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl; break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl; break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl; break;
		default:
			std::cout << "[unknown error code]" << std::endl;
		}
		error = true;
	}
	return error;
}
