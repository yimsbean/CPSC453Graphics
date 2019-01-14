/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"
#include "Data.h"
#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
	
	shaderProgram = ShaderTools::InitializeShaders();
	shaderProgramNoTs = ShaderTools::InitializeShadersWOTs();
	if (shaderProgram == 0) {
		std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
		return;
	}
	if (shaderProgramNoTs == 0) {
		std::cout << "Program could not initialize NoTs-shaders, TERMINATING" << std::endl;
		return;
	}
}

RenderingEngine::~RenderingEngine() { 

}

void RenderingEngine::RenderScene(const std::vector<std::vector<Geometry>> &objectsList, const std::vector<Geometry>& points) {
	//Clears the screen to a dark grey background
	glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	///////////////////////////////////////////////////// Objects(Bezier Curves)
	glUseProgram(shaderProgram);
	//// Specifically for Part 3 and Bonus1, Start
	if(Data::pointCount == 3)
		glPatchParameteri(GL_PATCH_VERTICES, 3);
	else
		glPatchParameteri(GL_PATCH_VERTICES, 4);
	if(Data::scEnabled){
		Data::currentTime = std::chrono::system_clock::now();
		if((Data::currentTime - Data::startTime).count() / 1000 > 0){
			Data::startTime = Data::currentTime;
		}
		if(Data::QuestionNumber == 2){ //
			Data::scLocation_x += 0.005*Data::scRate;
			if(Data::scLocation_x > Data::scMaxLength + 1)
				Data::scLocation_x = -1;
			if(Data::scLocation_x < -1)
				Data::scLocation_x = Data::scMaxLength+1;
		}else if(Data::QuestionNumber == 3){
			Data::scLocation_y -= 0.008*Data::scRate;
			if(Data::scLocation_y < -Data::scMaxLength-1)
				Data::scLocation_y = 1;
			if(Data::scLocation_y > +1)
				Data::scLocation_y = -Data::scMaxLength-1;
		}
	}
	movePixels = glGetUniformLocation(shaderProgram, "movePixels");
	glUniform2f(movePixels, Data::scLocation_x,Data::scLocation_y);
	//// Specifically for Part 3 and Bonus1, End

	for (const std::vector<Geometry>& objects : objectsList){
		for (const Geometry& g : objects) {
			glBindVertexArray(g.vao);
			glDrawArrays(g.drawMode, 0, g.verts.size());
			glBindVertexArray(0);
		}
	}
	
	glUseProgram(0);
	CheckGLErrors();
	///////////////////////////////////////////////////// Points (Q1)
	if(points.size() > 0){
		glUseProgram(shaderProgramNoTs);

		for (const Geometry& g : points) {
			glBindVertexArray(g.vao);
			glDrawArrays(g.drawMode, 0, g.verts.size());
			glBindVertexArray(0);
		}
		glUseProgram(0);
		CheckGLErrors();
	}
	moveFlag = false;
}

void RenderingEngine::assignBuffers(Geometry& geometry) {
	//Generate vao for the object
	//Constant 1 means 1 vao is being generated
	glGenVertexArrays(1, &geometry.vao);
	glBindVertexArray(geometry.vao);
	//Generate vbos for the object
	//Constant 1 means 1 vbo is being generated
	glGenBuffers(1, &geometry.vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	/*
	glGenBuffers(1, &geometry.normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(2);
	*/

	glGenBuffers(1, &geometry.colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	//Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);

	/*
	glGenBuffers(1, &geometry.uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	*/
}

void RenderingEngine::setBufferData(Geometry& geometry) {
	//Send geometry to the GPU
	//Must be called whenever anything is updated about the object
	glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);
	/*
	glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);
	*/
	glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);
	
	/*
	glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * geometry.uvs.size(), geometry.uvs.data(), GL_STATIC_DRAW);
	*/
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
