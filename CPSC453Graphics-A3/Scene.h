/*
 * Scene.h
 *	Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include <string>
#include "Geometry.h"
#include "GlyphExtractor.h"

//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
	Scene(RenderingEngine* renderer);
	virtual ~Scene();

	//Send geometry to the renderer
	void displayScene();

	void Initialize();

	void Q1_Scene1();	
	void Q1_Scene2();

	void Q2_Font1();
	void Q2_Font2();
	void Q2_Font3();
	void Q2_Font(GlyphExtractor);

	void Q3_Scroll1();
	void Q3_Scroll2();
	void Q3_Scroll3();
	void Q3_Scroll(GlyphExtractor);

	void RenderText(std::string, GlyphExtractor, bool);

	void Bonus1();
	void Bonus1Draw(std::string, GlyphExtractor, bool);
	void Bonus2();
private:
	RenderingEngine* renderer;

	Geometry drawing;
	Geometry point;
	//list of objects in the scene
	std::vector<Geometry> objects;
	std::vector<Geometry> points;

	std::vector<std::vector<Geometry>> objectsList;
};

#endif /* SCENE_H_ */
