/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"

#include <iostream>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/glu.h>

#include "Program.h"
#include "texture.h"

MyTexture back, gaussianTexture, texture;
GLuint fbo=0;	//framebuffer
GLint screenViewPort[4];
float sigma, calculatedConstant, sum;



Scene::Scene(GLFWwindow* window, RenderingEngine* renderer) : renderer(renderer) {
	const char* image = "images/image1-mandrill.png";
	switchImages(window,image, 0.0,0.0,1.0,0.0,1);	//default
}
Scene::~Scene() {

}
void Scene::displayBackScene() {
	renderer->RenderScene(background,1,fbo,renderer->shaderProgram2);
}
void Scene::displayGaussianScene() {
	renderer->RenderScene(objects,2, fbo,renderer->shaderProgram3);
}
void Scene::displayScene() {
	renderer->RenderScene(objects,3, fbo,renderer->shaderProgram);
}

#include <stdio.h>
Geometry rectangle;
void Scene::loadAndRenderImagesGaussian(GLFWwindow* window, const char* imageLocation,float x_diff, float y_diff,float factor, float radians, int gaussianKernelSize){	
	
	gaussianObjects.clear();
	rectangle  = Geometry();
	glGetIntegerv(GL_VIEWPORT,screenViewPort);
	glActiveTexture(GL_TEXTURE1);
	glUseProgram(renderer->shaderProgram3);
	glGetIntegerv(GL_VIEWPORT, screenViewPort);
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	InitializeTexture(&gaussianTexture, imageLocation, GL_TEXTURE_RECTANGLE);
	glViewport(0,0,gaussianTexture.width,gaussianTexture.height);
	glBindTexture(GL_TEXTURE_RECTANGLE, gaussianTexture.textureID);
	glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0, gaussianTexture.textureID,0);	
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
		printf("ERROR in Framebuffer\n");
		glBindFramebuffer(GL_FRAMEBUFFER,1);
	}

	//Get identifier for uniformshaderProgram
	GLuint uniformLocation 		= glGetUniformLocation(renderer->shaderProgram3, "imageTexture");
	GLuint render1stTime_v 		= glGetUniformLocation(renderer->shaderProgram3, "render1stTime_v");
	GLuint render1stTime_f 		= glGetUniformLocation(renderer->shaderProgram3, "render1stTime_f");
	glUniform1i(uniformLocation, 1);
	glUniform1i(render1stTime_v, 1);	
	glUniform1i(render1stTime_f, 1);	

	GLuint kernelConstant 		= glGetUniformLocation(renderer->shaderProgram3, "kernelConstant");
	GLuint kernelSize			= glGetUniformLocation(renderer-> shaderProgram3, "kernelSize");
	GLuint diff2 				= glGetUniformLocation(renderer->shaderProgram3, "diff2");
	sigma = 0.2 * gaussianKernelSize;
	calculatedConstant=pow(M_E,-1.0/(2*sigma*sigma))/(sigma*pow(2*M_PI,0.5));
	
	sum=0;
	for(int i=-(gaussianKernelSize)/2;i<=(gaussianKernelSize)/2; ++i){
		sum +=calculatedConstant*pow(M_E,-1*i*i);
	}
	glUniform1f(diff2, (1.0-sum)/(gaussianKernelSize));
	glUniform1f(kernelConstant, calculatedConstant);
	glUniform1i(kernelSize, gaussianKernelSize);




	if(renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed1" << std::endl;
	}
	
	rectangle.verts.push_back(glm::vec3( -1.0f, -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3(  1.0f, -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3(  1.0f,  1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( -1.0f, -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3(  1.0f,  1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( -1.0f,  1.0f, 1.0f));
	rectangle.drawMode = GL_TRIANGLES;
	
	rectangle.uvs.push_back(glm::vec2( 0.0f							, 0.0f								));
	rectangle.uvs.push_back(glm::vec2( float(gaussianTexture.width)	, 0.0f								));
	rectangle.uvs.push_back(glm::vec2( float(gaussianTexture.width)	, float(gaussianTexture.height)		));
	rectangle.uvs.push_back(glm::vec2( 0.0f							, 0.0f								));
	rectangle.uvs.push_back(glm::vec2( float(gaussianTexture.width)	, float(gaussianTexture.height) 	));
	rectangle.uvs.push_back(glm::vec2( 0.0f							, float(gaussianTexture.height) 	));
	
	RenderingEngine::assignBuffers(rectangle);
	RenderingEngine::setBufferData(rectangle);
	gaussianObjects.push_back(rectangle);
	
	//Render to framebuffer!
	for (const Geometry& g : gaussianObjects) {
		glBindVertexArray(g.vao);
		glDrawArrays(g.drawMode,0, g.verts.size());
		glBindVertexArray(0);
	}glBindFramebuffer(GL_FRAMEBUFFER,1);
	glUseProgram(0);
	renderer->CheckGLErrors();
}

void Scene::switchImages(GLFWwindow* window, const char* imageLocation, float x_diff, float y_diff,float factor, float radians, int gaussianKernelSize){
	loadAndRenderImagesGaussian(window, imageLocation,x_diff,  y_diff, factor,  radians,  gaussianKernelSize);
	glUseProgram(renderer->shaderProgram);
	glViewport(0,0,512,512);
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	objects.clear();
	glActiveTexture(GL_TEXTURE0);
	texture = gaussianTexture;
	glBindTexture(GL_TEXTURE_RECTANGLE, texture.textureID);
	
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		printf("Error in 2nd framebuffer\n");



	GLuint uniformLocation 		= glGetUniformLocation(renderer->shaderProgram, "imageTexture");
	GLuint sepiaLocation 		= glGetUniformLocation(renderer->shaderProgram, "sepiaTone");
	GLuint grayScaleLocation1 	= glGetUniformLocation(renderer->shaderProgram, "grayscale1");
	GLuint grayScaleLocation2 	= glGetUniformLocation(renderer->shaderProgram, "grayscale2");
	GLuint grayScaleLocation3 	= glGetUniformLocation(renderer->shaderProgram, "grayscale3");
	GLuint verticalLocation	 	= glGetUniformLocation(renderer->shaderProgram, "vertical");
	GLuint horizontalLocation 	= glGetUniformLocation(renderer->shaderProgram, "horizontal");
	GLuint unsharpLocation 		= glGetUniformLocation(renderer->shaderProgram, "unsharp");

	GLuint kernelConstant 		= glGetUniformLocation(renderer->shaderProgram, "kernelConstant");
	GLuint kernelSize 			= glGetUniformLocation(renderer->shaderProgram, "kernelSize");
	GLuint diff2 				= glGetUniformLocation(renderer->shaderProgram, "diff2");
	
	//Load texture unit number into uniform
	glUniform1i(uniformLocation		, 0);
	glUniform1i(sepiaLocation		, Global::sepia);
	glUniform1i(grayScaleLocation1	, Global::gray1);
	glUniform1i(grayScaleLocation2	, Global::gray2);
	glUniform1i(grayScaleLocation3	, Global::gray3);
	glUniform1i(verticalLocation	, Global::vert);
	glUniform1i(horizontalLocation	, Global::hori);
	glUniform1i(unsharpLocation		, Global::unsharp);
	
	glUniform1f(diff2, (1.0-sum)/(gaussianKernelSize));
	glUniform1f(kernelConstant, calculatedConstant);
	glUniform1i(kernelSize, gaussianKernelSize);

	GLuint rat = glGetUniformLocation(renderer->shaderProgram3,"ratio");
	GLuint fac = glGetUniformLocation(renderer->shaderProgram3,"factor");
	GLuint rad = glGetUniformLocation(renderer->shaderProgram3,"rads");
	GLuint pan = glGetUniformLocation(renderer->shaderProgram3,"diff");

	
	if(texture.width > texture.height)
		glUniform2f(rat, 1.0, (float)texture.height/texture.width);
	else
		glUniform2f(rat, (float)texture.width/texture.height, 1.0);
	
	glUniform1f(fac, factor);
	glUniform1f(rad, radians);
	glUniform2f(pan, x_diff/texture.width, y_diff/texture.height);
	//glUniform2f(pan, (1/factor)*x_diff/texture.width, (1/factor)*y_diff/texture.height);
	
	if(renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed2" << std::endl;
	}

	rectangle.verts.push_back(glm::vec3( -1.0f, -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( 1.0f,  -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( -1.0f, -1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( 1.0f, 1.0f, 1.0f));
	rectangle.verts.push_back(glm::vec3( -1.0f, 1.0f, 1.0f));

	rectangle.drawMode = GL_TRIANGLES;
	
	rectangle.uvs.push_back(glm::vec2( 0.0f						, 0.0f						));
	rectangle.uvs.push_back(glm::vec2( float(texture.width)		, 0.0f						));
	rectangle.uvs.push_back(glm::vec2( float(texture.width)		, float(texture.height)		));
	rectangle.uvs.push_back(glm::vec2( 0.0f						, 0.0f						));
	rectangle.uvs.push_back(glm::vec2( float(texture.width)		, float(texture.height) 	));
	rectangle.uvs.push_back(glm::vec2( 0.0f						, float(texture.height) 	));
	
	RenderingEngine::assignBuffers(rectangle);
	RenderingEngine::setBufferData(rectangle);
	objects.push_back(rectangle);
}

void Scene::switchImagesWithBackground(const char* backgroundLocation){
	background.clear();
	Geometry backRectangle  = Geometry();
	
	//Load texture uniform
	//Shaders need to be active to load uniformsglBindSampler(1,uniformLocation);
	glUseProgram(renderer->shaderProgram2);
	GLuint uniformLocation = glGetUniformLocation(renderer->shaderProgram2, "imageTexture");

	GLuint rat = glGetUniformLocation(renderer->shaderProgram2,"ratio");
	GLuint fac = glGetUniformLocation(renderer->shaderProgram2,"factor");
	GLuint rad = glGetUniformLocation(renderer->shaderProgram2,"rads");
	GLuint pan = glGetUniformLocation(renderer->shaderProgram2,"diff");

/////////////////////////////////////////
	//Set which texture unit the texture is bound to
	
	glActiveTexture(GL_TEXTURE2);
	InitializeTexture(&back, backgroundLocation, GL_TEXTURE_RECTANGLE);
	//Bind the texture to GL_TEXTURE0
	glBindTexture(GL_TEXTURE_RECTANGLE, back.textureID);
	//Get identifier for uniformshaderProgram
	//Load texture unit number into uniform
	glUniform1i(uniformLocation,2);
	
	glUniform2f(rat, 1.0, 1.0);
	glUniform1f(fac, 1.0);
	glUniform1f(rad, 0.0);
	glUniform2f(pan, 0.0,0.0);
	
	if(renderer->CheckGLErrors()) {
		std::cout << "Texture creation failed3" << std::endl;
	}	
	// three vertex positions and assocated colours of a triangle

	backRectangle.verts.push_back(glm::vec3( -0.9f, -0.9f, 1.0f));
	backRectangle.verts.push_back(glm::vec3( 0.9f,  -0.9f, 1.0f));
	backRectangle.verts.push_back(glm::vec3( 0.9f, 0.9f, 1.0f));
	backRectangle.verts.push_back(glm::vec3( -0.9f, -0.9f, 1.0f));
	backRectangle.verts.push_back(glm::vec3( 0.9f, 0.9f, 1.0f));
	backRectangle.verts.push_back(glm::vec3( -0.9f, 0.9f, 1.0f));

	backRectangle.drawMode = GL_TRIANGLES;
	backRectangle.uvs.push_back(glm::vec2( 0.0f					, 0.0f		));
	backRectangle.uvs.push_back(glm::vec2( float(back.width)	, 0.0f		));
	backRectangle.uvs.push_back(glm::vec2( float(back.width)	,  float(back.height)	));
	backRectangle.uvs.push_back(glm::vec2( 0.0f					, 0.0f		));
	backRectangle.uvs.push_back(glm::vec2( float(back.width)	,  float(back.height) 	));
	backRectangle.uvs.push_back(glm::vec2( 0.0f					,  float(back.height)	));
	//Construct vao and vbos for the triangle
	RenderingEngine::assignBuffers(backRectangle);
	//Send the triangle data to the GPU
	//Must be done every time the triangle is modified in any way, ex. verts, colors, normals, uvs, etc.
	RenderingEngine::setBufferData(backRectangle);
	//Add the triangle to the scene objects
	background.push_back(backRectangle);
}