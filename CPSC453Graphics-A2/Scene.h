/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>

#include "Geometry.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(GLFWwindow*, RenderingEngine* renderer);
	virtual ~Scene();
	void loadAndRenderImagesGaussian(GLFWwindow*, const char* ,float , float ,float , float , int );
	void switchImages(GLFWwindow*, const char *,float,float,float,float,int);
	void switchImagesWithBackground(const char *);
	//Send geometry to the renderer
	void displayBackScene();
	void displayGaussianScene();
	void displayScene();

private:
	RenderingEngine* renderer;
	//Geometry rectangle;
	//Geometry backRectangle;
	//list of objects in the scene
	std::vector<Geometry> gaussianObjects;
	std::vector<Geometry> objects;
	std::vector<Geometry> background;
};

#endif /* SCENE_H_ */
