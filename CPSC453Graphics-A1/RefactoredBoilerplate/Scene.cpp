/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

 //**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cmath> 
#include <random>
#include <vector>

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	changeTo_SquaresAndDiamons(1);
}

Scene::~Scene() {

}

void Scene::displayScene() {
	renderer->RenderScene(objects);
}

//Q1. Fin
void Scene::changeTo_SquaresAndDiamons(int lv) { //Allow at least 6 level of nesting

	objects.clear();
	double point[4][2] = { {-0.5f,-0.5f},{0.5f,-0.5f},{0.5f,0.5f},{-0.5f,0.5f} };	//x, y
	double temp_x, temp_y;
	for (int level = 0; level < lv*2; ++level) {
		Geometry squaresAndDiamonds;
		squaresAndDiamonds.drawMode = GL_LINE_STRIP;
		squaresAndDiamonds.verts.push_back(glm::vec3(point[0][0], point[0][1], 1.0f));
		squaresAndDiamonds.verts.push_back(glm::vec3(point[1][0], point[1][1], 1.0f));
		squaresAndDiamonds.verts.push_back(glm::vec3(point[2][0], point[2][1], 1.0f));
		squaresAndDiamonds.verts.push_back(glm::vec3(point[3][0], point[3][1], 1.0f));
		squaresAndDiamonds.verts.push_back(glm::vec3(point[0][0], point[0][1], 1.0f));
		temp_x = point[0][0], temp_y = point[0][1];
		for (int i = 0; i < 3; ++i) {
			point[i][0] = (point[i][0] + point[i + 1][0]) / 2;
			point[i][1] = (point[i][1] + point[i + 1][1]) / 2;
		}
		point[3][0] = (point[3][0] + temp_x) / 2; point[3][1] = (point[3][1] + temp_y) / 2;
		for (int i = 0; i < 5; ++i) {
			if (level % 2 == 0)
				squaresAndDiamonds.colors.push_back(glm::vec3(1.0f + (level) / (lv + 1), 1.0f*(lv - level) / (lv + 1), 0.0f));
			else
				squaresAndDiamonds.colors.push_back(glm::vec3(0.0f, 1.0f*(lv - level) / (lv + 1), 1.0f*(level) / (lv + 1)));
		}
		RenderingEngine::assignBuffers(squaresAndDiamonds);
		RenderingEngine::setBufferData(squaresAndDiamonds);
		objects.push_back(squaresAndDiamonds);
	}
}
//Q2. Fin
void Scene::changeTo_ParametricSpiral(int lv) {
	objects.clear();
	Geometry parametricSprial;
	parametricSprial.drawMode = GL_LINE_STRIP;
	int numSegments = 50;
	float du = 2 * M_PI / numSegments;
	for (float u = 0; u <= 2 * lv * M_PI; u += du) {
		//vertex at this value of u
		parametricSprial.verts.push_back(glm::vec3((u / (2 * M_PI*lv)) * cos(u), (u / (2 * M_PI*lv))* sin(u), 1.0));
		//color at this value of u
		if ((lv%2 ==0 && (int)(u / (2 * M_PI)) % 2 == 0) || (lv%2 != 0 && (int)(u / (2 * M_PI)) % 2 != 0))
			parametricSprial.colors.push_back(glm::vec3((1.0f*(int)(u / (2 * M_PI)) + 1) / (lv + 1), (1.0f*(int)(u / (2 * M_PI)) + 1) / (lv + 1), 0.0f));
		else
			parametricSprial.colors.push_back(glm::vec3(0.0f, (1.0f*(int)(u / (2 * M_PI)) + 1) / (lv + 1), (1.0f*(int)((u / (2 * M_PI))) + 1) / (lv + 1)));
		RenderingEngine::assignBuffers(parametricSprial);
		RenderingEngine::setBufferData(parametricSprial);
		objects.push_back(parametricSprial);
	}
}

//Q3-a. Colouring
void Scene::sierpinski_recursive(double point[][2], int lv) {
	if (lv > 0) {
		double p1[3][2] = { {point[0][0], point[0][1]}											,{(point[0][0] + point[1][0]) / 2, (point[0][1] + point[1][1]) / 2}	,{(point[0][0] + point[2][0]) / 2	, (point[0][1] + point[2][1]) / 2} };
		double p2[3][2] = { {(point[0][0] + point[1][0]) / 2, (point[0][1] + point[1][1]) / 2}	,{point[1][0], point[1][1]}											,{(point[1][0] + point[2][0]) / 2	, (point[1][1] + point[2][1]) / 2} };
		double p3[3][2] = { {(point[0][0] + point[2][0]) / 2, (point[0][1] + point[2][1]) / 2}	,{(point[1][0] + point[2][0]) / 2, (point[1][1] + point[2][1]) / 2}	,{point[2][0]					, point[2][1]} };
		sierpinski_recursive(p1, lv - 1);
		sierpinski_recursive(p2, lv - 1);
		sierpinski_recursive(p3, lv - 1);
		return;
	}
	Geometry triangle;

	triangle.verts.push_back(glm::vec3(point[0][0], point[0][1], 1.0f));
	triangle.verts.push_back(glm::vec3(point[1][0], point[1][1], 1.0f));
	triangle.verts.push_back(glm::vec3(point[2][0], point[2][1], 1.0f));
	double centre_x=(point[0][0]+point[1][0]+point[2][0])/3;
	double centre_y=(point[0][1]+point[1][1]+point[2][1])/3;
	glm::vec3 colour = glm::vec3(1.0f-distance(centre_x, -1.0f, centre_y, -0.75f),1.0f-distance(centre_x, 1.0f, centre_y, -0.75f),1.0f-distance(centre_x, 0.0f, centre_y, 1.75f*cos(M_PI / 3.0f)));

	for (int i = 0; i < 3; ++i)
		triangle.colors.push_back(colour);
	triangle.drawMode = GL_TRIANGLES;

	RenderingEngine::assignBuffers(triangle);
	RenderingEngine::setBufferData(triangle);
	objects.push_back(triangle);
}
double Scene::distance(double x1, double x2, double y1, double y2){
	return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}
void Scene::changeTo_SierpinskiTriangle(int lv) {
	objects.clear();
	double point[3][2] = { {-1.0f,-0.75f},{1.0f,-0.75f},{0.0f,1.75f*cos(M_PI / 3.0f)} };
	sierpinski_recursive(point, lv);
}
//Q3-b. Colouring
void Scene::mengerSponge_recursive(double point[2], double len, int pos, int deg, int lv) { //bottom-left position as point
	if (lv > 0) {
		double p1[2] = {point[0]		, point[1]};
		double p2[2] = {point[0]+len	, point[1]};
		double p3[2] = {point[0]+len*2	, point[1]};
		double p4[2] = {point[0]+len*2	, point[1]+len};
		double p5[2] = {point[0]+len*2	, point[1]+len*2};
		double p6[2] = {point[0]+len	, point[1]+len*2};
		double p7[2] = {point[0]		, point[1]+len*2};
		double p8[2] = {point[0]		, point[1]+len};

		mengerSponge_recursive(p1, len/3, 1, deg	, lv - 1);
		mengerSponge_recursive(p2, len/3, 2, deg+1	, lv - 1);
		mengerSponge_recursive(p3, len/3, 3, deg	, lv - 1);
		mengerSponge_recursive(p4, len/3, 4, deg+1	, lv - 1);
		mengerSponge_recursive(p5, len/3, 5, deg	, lv - 1);
		mengerSponge_recursive(p6, len/3, 6, deg+1	, lv - 1);
		mengerSponge_recursive(p7, len/3, 7, deg	, lv - 1);
		mengerSponge_recursive(p8, len/3, 8, deg+1	, lv - 1);

		return;
	}
	Geometry sponge;

	sponge.verts.push_back(glm::vec3(point[0], point[1], 1.0f));			//Bottom-left
	sponge.verts.push_back(glm::vec3(point[0]+len*3, point[1], 1.0f));		//Bottom-right
	sponge.verts.push_back(glm::vec3(point[0]+len*3, point[1]+len*3, 1.0f));//Top-right
	sponge.verts.push_back(glm::vec3(point[0], point[1]+len*3, 1.0f));		//Top-left
	glm::vec3 colour;
	switch(pos){
		case 0: case 2: case 4: case 6: case 8: colour = glm::vec3((1-1.0f/(deg+1.5f)),(1-1.0f/(deg+1.5f)),(1-1.0f/(deg+1.5f))); break;
		case 1: colour = glm::vec3((1-1.0f/(deg+1.5f)),0.0f,0.0f); break;
		case 3: colour = glm::vec3(0.0f,(1-1.0f/(deg+1.5f)),0.0f); break;
		case 5: colour = glm::vec3(0.0f,0.0f,(1-1.0f/(deg+1.5f))); break;
		case 7: colour = glm::vec3((1-1.0f/(deg+1.5f)),0.0f,(1-1.0f/(deg+1.5f))); break;
	}

	for (int i = 0; i < 4; ++i)
		sponge.colors.push_back(colour);
	sponge.drawMode=GL_TRIANGLE_FAN;

	RenderingEngine::assignBuffers(sponge);
	RenderingEngine::setBufferData(sponge);
	objects.push_back(sponge);
}
void Scene::changeTo_MengerSponge(int lv) { //do it after bonus Q
	objects.clear();
	double point[2] = { -1.0f,-1.0f};	//x, y
	mengerSponge_recursive(point,2.0f/3, 0, 0,lv);
}
//Q4. Fin.
void Scene::changeTo_SierpinskiTriangleReloaded(int lv) {//WIP
	objects.clear();
	long dotCount = pow(2, lv);	//2^n dots

	Geometry dots;
	double originallVertices[3][2] = { {-1.0f,-0.75f},{1.0f,-0.75f},{0.0f,1.75f*cos(M_PI / 3.0f)} };
	double oldVertex[2];
	double newVertex[2];
	//Rand Initialization Start
	std::random_device rd;
	std::mt19937 RNG{ rd() };
	std::uniform_int_distribution<int> sides(0, 2);// random pick from triangle's one side {0,1,2}]
	//Rand Initialization End
	//oldVertex init
	switch (sides(RNG)) {
	case 0: oldVertex[0] = originallVertices[0][0]; 	oldVertex[1] = originallVertices[0][1]; 	break;
	case 1: oldVertex[0] = originallVertices[1][0]; 	oldVertex[1] = originallVertices[1][1]; 	break;
	case 2: oldVertex[0] = originallVertices[2][0]; 	oldVertex[1] = originallVertices[2][1]; 	break;
	}
	//newvertex Loop
	for (long i = 0; i < dotCount; ++i) {
		dots.verts.push_back(glm::vec3(oldVertex[0], oldVertex[1], 1.0f));
		dots.colors.push_back(glm::vec3(0.5f, 0.5f, 0.5f));
		switch (sides(RNG)) {
		case 0: newVertex[0] = originallVertices[0][0]; 	newVertex[1] = originallVertices[0][1]; 	break;
		case 1: newVertex[0] = originallVertices[1][0]; 	newVertex[1] = originallVertices[1][1]; 	break;
		case 2: newVertex[0] = originallVertices[2][0]; 	newVertex[1] = originallVertices[2][1]; 	break;
		}
		oldVertex[0] = (oldVertex[0] + newVertex[0]) / 2; 	oldVertex[1] = (oldVertex[1] + newVertex[1]) / 2;
	}
	dots.drawMode = GL_POINTS;

	RenderingEngine::assignBuffers(dots);
	RenderingEngine::setBufferData(dots);
	objects.push_back(dots);
}

//Q5. Additional
void Scene::barnsleyFernRecursive(double x1, double y1, int lv, double& max_x, double& max_y, std::vector<double>& list_x, std::vector<double>& list_y){
	double x2=0,y2=0;
	if(lv > 0){
		//Rand Initialization Start
		//Rand Initialization End
		std::random_device rd;
		std::mt19937 RNG{ rd() };
		std::uniform_int_distribution<int> uid(0, 99);
		int rand;
		rand=uid(RNG);
		if(rand==0){					//1% F1
			x2=0; y2=0.16*y1;
		}else if(rand>=1 && rand<=85){	//85% f2
			x2=0.85*x1+0.04*y1; y2=-0.04*x1+0.85*y1+1.6;
		}else if(rand>=86 && rand<=92){	//7% f3
			x2=0.2*x1-0.26*y1; y2=0.23*x1+0.22*y1+1.6;
		}else if(rand>=93){				//7% f4
			x2=-0.15*x1+0.28*y1; y2=0.26*x1+0.24*y1+0.44;
		}
		if(max_x < x2) max_x=x2;
		if(max_y < y2) max_y=y2;
		list_x.push_back(x2);
		list_y.push_back(y2);
		barnsleyFernRecursive(x2, y2, lv-1, max_x, max_y, list_x, list_y);
	}
}

void Scene::changeTo_BarnsleyFern(int lv) {//WIP
	objects.clear();
	Geometry polygon;
	polygon.drawMode = GL_POINTS;
	double max_x = 0, max_y = 0;
	std::vector<double> list_x;
	std::vector<double> list_y;
	barnsleyFernRecursive(0,0,pow(2,lv), max_x, max_y, list_x, list_y);
	for(int i=0; i< list_x.size();++i){
		polygon.verts.push_back(glm::vec3(list_x[i]/max_x,list_y[i]/max_y,1.0f));
		polygon.colors.push_back(glm::vec3(0.0f,0.8f,0.0f));
	}
	RenderingEngine::assignBuffers(polygon);
	RenderingEngine::setBufferData(polygon);
	objects.push_back(polygon);
}
//Q5. Additional
//convert d to (x,y)
void Scene::hilbert_d2xy(int n, int d, int *x, int *y) {
    int rx, ry, s, t=d;
    *x = *y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        hilbert_rot(s, x, y, rx, ry);
        *x += s * rx;
        *y += s * ry;
        t /= 4;
    }
}
//rotate/flip a quadrant appropriately
void Scene::hilbert_rot(int n, int *x, int *y, int rx, int ry) {
    if (ry == 0) {
        if (rx == 1) {
            *x = n-1 - *x;
            *y = n-1 - *y;
        }
        //Swap x and y
        int t  = *x;
        *x = *y;
        *y = t;
    }
}
void Scene::changeTo_HilbertSpaceFillingCurve(int lv) {//WIP
	objects.clear();
	double nextlen;
	if(lv == 0)
		nextlen = 0.5;
	else
		nextlen = pow(2,lv) - 1.0f/pow(2,lv);
	int hil_x=0, hil_y=0;
	Geometry polygon;
	polygon.drawMode = GL_LINE_STRIP;
	for(int i=0; i <pow(4,lv+1); ++i){
		hilbert_d2xy( pow(2,lv+1), i, &hil_x, &hil_y);
		polygon.verts.push_back(glm::vec3(-(float)hil_x/pow(2,lv+1)+nextlen/pow(2,lv+1),-(float)hil_y/pow(2,lv+1)+nextlen/pow(2,lv+1),1.0f));
		polygon.colors.push_back(glm::vec3(0.5f,0.5f,0.5f));
	}
	RenderingEngine::assignBuffers(polygon);
	RenderingEngine::setBufferData(polygon);
	objects.push_back(polygon);
}
