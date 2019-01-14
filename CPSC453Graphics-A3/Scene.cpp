/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 */

#include "Scene.h"
#include "Data.h"

#include <iostream>

#include "RenderingEngine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLuint pointCount, questionNo;

Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
	Q1_Scene1();
}
Scene::~Scene() {

}
void Scene::displayScene() {
	renderer->RenderScene(objectsList, points);
}

void Scene::Initialize(){
	Data::scEnabled = false;
	objects.clear();
	points.clear();
	objectsList.clear();
	drawing = Geometry();
	drawing.drawMode = GL_PATCHES;
	point = Geometry();
	point.drawMode = GL_POINTS;

	Data::scLocation_x = 0;
	Data::scLocation_y = 0;
	glEnable(GL_PROGRAM_POINT_SIZE);
	glUseProgram(renderer->shaderProgram);
	pointCount = glGetUniformLocation(renderer->shaderProgram, "pointCount");
	questionNo = glGetUniformLocation(renderer->shaderProgram, "questionNo");
	glUniform1i(questionNo, 1);
}
////////////////////////////////////////// Q1
void Scene::Q1_Scene1(){
	Initialize();
	Data::pointCount = 3;
	glPatchParameteri(GL_PATCH_VERTICES, 3); 
	glUniform1i(pointCount, 3);
	float 	x_min = Data::Q1_vertices1[0][0], x_max = Data::Q1_vertices1[0][0],
			y_min = Data::Q1_vertices1[0][1], y_max = Data::Q1_vertices1[0][1];
	for(int i=0; i<12; ++i) {
		if(Data::Q1_vertices1[i][0] < x_min)	x_min = Data::Q1_vertices1[i][0];
		if(Data::Q1_vertices1[i][0] > x_max)	x_max = Data::Q1_vertices1[i][0];
		if(Data::Q1_vertices1[i][1] < y_min)	y_min = Data::Q1_vertices1[i][1];
		if(Data::Q1_vertices1[i][1] > y_max)	y_max = Data::Q1_vertices1[i][1];
		drawing.verts.push_back(glm::vec3(Data::Q1_vertices1[i][0],Data::Q1_vertices1[i][1], 1.0f ));
		//drawing.uvs.push_back(glm::vec3(Data::Q1_vertices1[i][0],Data::Q1_vertices1[i][1], 1.0f ));
		point.verts.push_back(  glm::vec3(Data::Q1_vertices1[i][0],Data::Q1_vertices1[i][1], 1.0f ));
		switch((i/3)%3){
			case 0: drawing.colors.push_back(glm::vec3( (i+1)/13.0f	, 0.49f		    , 0.49f)); 		
					point.colors.push_back(	 glm::vec3( (i+1)/13.0f	, 0.49f		    , 0.49f)); break;
			case 1: drawing.colors.push_back(glm::vec3( 0.49f	    , (i+1)/13.0f	, 0.49f)); 		
					point.colors.push_back(	 glm::vec3( 0.49f	    , (i+1)/13.0f	, 0.49f)); break;
			case 2: drawing.colors.push_back(glm::vec3( 0.49f	    , 0.49f		    , (i+1)/13.0f));
					point.colors.push_back(	 glm::vec3( 0.49f	    , 0.49f			, (i+1)/13.0f));break;
		}
	}
	float x_diff = abs(x_max - x_min)/2.0, y_diff = abs(y_max - y_min)/2.0;
	bool isX = x_diff > y_diff ? true : false ;
	float diff = isX ? x_diff : y_diff;
	x_diff = x_max - x_diff; y_diff = y_max - y_diff;
	for(int i=0; i<12; ++i) {
		drawing.verts[i] = glm::vec3((drawing.verts[i].x-x_diff)/diff, (drawing.verts[i].y-y_diff)/diff, 1.0f );
		drawing.uvs.push_back(drawing.verts[i]);
		point.verts[i] = glm::vec3(	 (point.verts[i].x  -x_diff)/diff, (point.verts[i].y  -y_diff)/diff, 1.0f );
	}

	RenderingEngine::assignBuffers(drawing);
	RenderingEngine::setBufferData(drawing);
	objects.push_back(drawing);
	objectsList.push_back(objects);
	glUseProgram(renderer->shaderProgramNoTs);
	RenderingEngine::assignBuffers(point);
	RenderingEngine::setBufferData(point);
	points.push_back(point);
}
void Scene::Q1_Scene2(){
	Initialize();
	Data::pointCount = 4;
	glPatchParameteri(GL_PATCH_VERTICES, 4); 
	glUniform1i(pointCount, 4);
	float 	x_min = Data::Q1_vertices2[0][0], x_max = Data::Q1_vertices2[0][0],
			y_min = Data::Q1_vertices2[0][1], y_max = Data::Q1_vertices2[0][1];
	for(int i=0; i<20; ++i) {
		if(Data::Q1_vertices2[i][0] < x_min)	x_min = Data::Q1_vertices2[i][0];
		if(Data::Q1_vertices2[i][0] > x_max)	x_max = Data::Q1_vertices2[i][0];
		if(Data::Q1_vertices2[i][1] < y_min)	y_min = Data::Q1_vertices2[i][1];
		if(Data::Q1_vertices2[i][1] > y_max)	y_max = Data::Q1_vertices2[i][1];
		drawing.verts.push_back(glm::vec3( Data::Q1_vertices2[i][0], Data::Q1_vertices2[i][1], 1.0f ));
		point.verts.push_back(  glm::vec3( Data::Q1_vertices2[i][0], Data::Q1_vertices2[i][1], 1.0f ));
		switch((i/4)%4){
			case 0: drawing.colors.push_back(glm::vec3( (i+1)/13.0f	, 0.49f		    , 0.49f)); 		
					point.colors.push_back(	 glm::vec3( (i+1)/13.0f	, 0.49f		    , 0.49f)); break;
			case 1: drawing.colors.push_back(glm::vec3( 0.49f	    , (i+1)/13.0f	, 0.49f)); 		
					point.colors.push_back(	 glm::vec3( 0.49f	    , (i+1)/13.0f	, 0.49f)); break;
			case 2: drawing.colors.push_back(glm::vec3( 0.49f	    , 0.49f		    , (i+1)/13.0f));
					point.colors.push_back(	 glm::vec3( 0.49f	    , 0.49f			, (i+1)/13.0f));break;
			case 3: drawing.colors.push_back(glm::vec3( (i+1)/13.0f	, 0.49f		    , (i+1)/13.0f));
					point.colors.push_back(	 glm::vec3( (i+1)/13.0f , 0.49f			, (i+1)/13.0f));break;
		}
	}
	float x_diff = abs(x_max - x_min)/2.0, y_diff = abs(y_max - y_min)/2.0;
	bool isX = x_diff > y_diff ? true : false ;
	float diff = isX ? x_diff : y_diff;
	x_diff = x_max - x_diff; y_diff = y_max - y_diff;
	for(int i=0; i<20; ++i) {
		drawing.verts[i] = glm::vec3((drawing.verts[i].x-x_diff)/diff, (drawing.verts[i].y-y_diff)/diff, 1.0f );
		point.verts[i] = glm::vec3(	 (point.verts[i].x  -x_diff)/diff, (point.verts[i].y  -y_diff)/diff, 1.0f );
	}
	RenderingEngine::assignBuffers(drawing);
	RenderingEngine::setBufferData(drawing);
	objects.push_back(drawing);
	objectsList.push_back(objects);
	glUseProgram(renderer->shaderProgramNoTs);
	RenderingEngine::assignBuffers(point);
	RenderingEngine::setBufferData(point);
	points.push_back(point);
}
////////////////////////////////////////// Q2
void Scene::Q2_Font1(){	//Lora font
	Initialize();
	Data::pointCount = 3;
	GlyphExtractor g; g.LoadFontFile("fonts/lora/Lora-Regular.ttf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	Q2_Font(g);
}
void Scene::Q2_Font2(){	//SourceCodePro font
	Initialize();
	Data::pointCount = 4;
	GlyphExtractor g; g.LoadFontFile("fonts/source-sans-pro/SourceSansPro-Bold.otf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	Q2_Font(g);
}
void Scene::Q2_Font3(){ //font as i wish
	Initialize();
	Data::pointCount = 4;
	GlyphExtractor g; g.LoadFontFile("fonts/great-vibes/GreatVibes-Regular.otf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	Q2_Font(g);
}
void Scene::Q2_Font(GlyphExtractor g){ 
	RenderText(Data::name, g, true);
}
////////////////////////////////////////// Q3
void Scene::Q3_Scroll1(){ //AlexBrush Font
	Initialize();
	Data::pointCount = 3;
	GlyphExtractor g; g.LoadFontFile("fonts/alex-brush/AlexBrush-Regular.ttf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	Q3_Scroll(g);
}
void Scene::Q3_Scroll2(){	//Inconsolata Font
	Initialize();
	Data::pointCount = 4;
	GlyphExtractor g; g.LoadFontFile("fonts/inconsolata/Inconsolata.otf");
	glPatchParameteri(GL_PATCH_VERTICES,Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	Q3_Scroll(g);
}
void Scene::Q3_Scroll3(){ //font as i wish
	Initialize();
	Data::pointCount = 3;
	GlyphExtractor g; g.LoadFontFile("fonts/amatic/AmaticSC-Regular.ttf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount);glUniform1i(pointCount, Data::pointCount);
	Q3_Scroll(g);
}
void Scene::Q3_Scroll(GlyphExtractor g){
	Data::scEnabled = true;
	Data::startTime = std::chrono::system_clock::now();
	Data::currentTime = Data::startTime;
	RenderText(Data::scrollText, g, false);
}
// for part2, part3
void Scene::RenderText(std::string in, GlyphExtractor g, bool doFitIn){
	MyGlyph ch = g.ExtractGlyph(in[0]);
	float 	x_min = ch.contours[0][0].x[0], x_max = ch.contours[0][0].x[0],
			y_min = ch.contours[0][0].y[0], y_max = ch.contours[0][0].y[0];
	int vertexCount = 0;
	float pos;
	for( int m = 0; m < in.length() ; ++m){
		ch = g.ExtractGlyph(in[m]);
		if(doFitIn)
			pos = -1 - 1.0/in.length() + 2.0*m/in.length();		//Part 2
		else
			pos =  17.0*m/in.length();							//Part 3
		for (unsigned int i = 0; i < ch.contours.size(); i++) {
			for(unsigned int j = 0; j < ch.contours[i].size(); j++) {
				for(unsigned int k = 0; k <= ch.contours[i][j].degree; k++) {
					if(ch.contours[i][j].x[k]+pos < x_min)	x_min = ch.contours[i][j].x[k]+pos;
					if(ch.contours[i][j].x[k]+pos > x_max)	x_max = ch.contours[i][j].x[k]+pos;
					if(ch.contours[i][j].y[k] < y_min)	y_min = ch.contours[i][j].y[k];
					if(ch.contours[i][j].y[k] > y_max)	y_max = ch.contours[i][j].y[k];
					
					drawing.verts.push_back(glm::vec3( 
						ch.contours[i][j].x[k]+pos, 
						ch.contours[i][j].y[k], 
						1.0f 
					));
					drawing.colors.push_back(glm::vec3( 0.8f, 0.4f, 0.0f )); 
					vertexCount++;
				}
				if(Data::pointCount == 3 && ch.contours[i][j].degree == 1){
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
				}else if(Data::pointCount == 4 && ch.contours[i][j].degree == 2){
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
				}else if(Data::pointCount == 4 && ch.contours[i][j].degree == 1){
					drawing.verts.push_back(drawing.verts.back());
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
					drawing.colors.push_back(drawing.colors.back());
				}
			}
		}
		objects.push_back(drawing);
		objectsList.push_back(objects);
		objects.clear();
	}
	
	if(doFitIn){		//part2
		float x_diff = abs(x_max - x_min)/2.0, y_diff = abs(y_max - y_min)/2.0;
		bool isX = x_diff > y_diff ? true : false;
		float diff = isX ? x_diff : y_diff;
		x_diff = x_max - x_diff; y_diff = y_max - y_diff;
		for(std::vector<Geometry>& obj : objectsList)
			for (Geometry& g : obj) {
				for(glm::vec3& v : g.verts)
					v = glm::vec3((v.x-x_diff)/diff, (v.y-y_diff)/diff, 1.0f );
				RenderingEngine::assignBuffers(g);
				RenderingEngine::setBufferData(g);
			}
	}else{				//part3
		for(std::vector<Geometry>& obj : objectsList)
			for (Geometry& g : obj) {
				RenderingEngine::assignBuffers(g);
				RenderingEngine::setBufferData(g);
			}
	}
	Data::scMaxLength = x_max - x_min;
	
}

////////////////////////////////////////// Bonus1
int lineCount = 0;
void Scene::Bonus1(){ //Advanced Rendering
	Initialize();
	Data::pointCount = 4;
	GlyphExtractor g; g.LoadFontFile("fonts/inconsolata/Inconsolata.otf");
	glPatchParameteri(GL_PATCH_VERTICES, Data::pointCount); glUniform1i(pointCount, Data::pointCount);
	glUniform1i(questionNo, 4);
	
	Data::scEnabled = true;
	Data::startTime = std::chrono::system_clock::now();
	Data::currentTime = Data::startTime;
	lineCount = 0;
	////////////////////////////////////// 1st string, "Episode IV" , centre
	Bonus1Draw(Data::bonus1Text[0], g, true); lineCount++;
	////////////////////////////////////// 2nd string, "A NEW HOPE" , centre
	Bonus1Draw(Data::bonus1Text[1], g, true); 
	////////////////////////////////////// 3rd .. string, "It is a peirod ..." , left, fit screen
	for(int i=2; i<Data::bonus1Text.size(); ++i){
		lineCount++;lineCount++;lineCount++;
		Bonus1Draw(Data::bonus1Text[i], g, false);
	}
	for(std::vector<Geometry>& obj : objectsList)
		for (Geometry& g : obj) {
			RenderingEngine::assignBuffers(g);
			RenderingEngine::setBufferData(g);
	}
	Data::scMaxLength = 0.1*lineCount;
}
void Scene::Bonus1Draw(std::string in, GlyphExtractor g, bool isCentre){
	MyGlyph ch; float pos;
	for( int m = 0; m < in.length() ; ++m){
		if((m+1) % 30 == 0)			//newline
			lineCount++;
		ch = g.ExtractGlyph(in[m]);
		if(isCentre)
			pos = -1 + 0.5/in.length() + 1.2*((m+1)%30)/in.length();		//Line 1,2
		else
			pos = -1 + 0.5/30 + 2.0*((m+1)%30)/30;			//Line 3 ..
		for (unsigned int i = 0; i < ch.contours.size(); i++) {
			for(unsigned int j = 0; j < ch.contours[i].size(); j++) {
				for(unsigned int k = 0; k <= ch.contours[i][j].degree; k++) {					
					drawing.verts.push_back(glm::vec3( 
						ch.contours[i][j].x[k]/12 + pos, 
						ch.contours[i][j].y[k]/12 - 0.1*lineCount, 
						1.0f 
					));
					drawing.colors.push_back(glm::vec3( 0.8f, 0.4f, 0.0f )); 
				}
				if(Data::pointCount == 3 && ch.contours[i][j].degree == 1){
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
				}else if(Data::pointCount == 4 && ch.contours[i][j].degree == 2){
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
				}else if(Data::pointCount == 4 && ch.contours[i][j].degree == 1){
					drawing.verts.push_back(drawing.verts.back());
					drawing.verts.push_back(drawing.verts.back());
					drawing.colors.push_back(drawing.colors.back());
					drawing.colors.push_back(drawing.colors.back());
				}
			}
		}
		objects.push_back(drawing);
		objectsList.push_back(objects);
		objects.clear();
	}
}


////////////////////////////////////////// Bonus2
void Scene::Bonus2(){ //Font Editing
	Initialize();
	Data::scEnabled = true;
	Data::pointCount = 4;
	glPatchParameteri(GL_PATCH_VERTICES, 3); glUniform1i(pointCount, 3);
	
}


