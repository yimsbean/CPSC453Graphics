/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_
#define _USE_MATH_DEFINES

#include <vector>

#include "Geometry.h"

 //Forward declaration of classes
 //(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)

class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	void changeTo_SquaresAndDiamons(int);
	void changeTo_ParametricSpiral(int);
	void sierpinski_recursive(double[][2], int);
	double distance(double, double, double, double);
	void changeTo_SierpinskiTriangle(int);
	void mengerSponge_recursive(double[], double, int, int, int);
	void changeTo_MengerSponge(int);
	void changeTo_SierpinskiTriangleReloaded(int);

	void barnsleyFernRecursive(double,double, int);
	void barnsleyFernRecursive(double x1, double y1, int lv, double& max_x, double & max_y, std::vector<double>& list_x, std::vector<double>& list_y);
	void changeTo_BarnsleyFern(int);
	void hilbert_d2xy(int, int, int*, int*);
	void hilbert_rot(int, int*, int*, int, int);
	void changeTo_HilbertSpaceFillingCurve(int);

private:
	RenderingEngine* renderer;

	//list of objects in the scene
	std::vector<Geometry> objects;
};

#endif /* SCENE_H_ */
