/*
 * Scene.h
 *  Class for storing objects in a scene
 *  Created on: Sep 10, 2018
 *      Author: cb-ha
 */

#ifndef SCENE_H_
#define SCENE_H_

// std
#include <vector>
#include <iostream>
// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//**Must include glad and GLFW in this order or it breaks**
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//customs
#include "RenderingEngine.h"
#include "Camera.h"
#include "Node.h"
#include "Geometry.h"
#include "Node.h"

#define _USE_MATH_DEF
#include "Math.h"


//Forward declaration of classes
//(note this is necessary because these are pointers and it allows the #include to appear in the .cpp file)
class RenderingEngine;

class Scene {
public:
    Scene(RenderingEngine* renderer);
    virtual ~Scene();

    static const int WIDTH = 1100;
    static const int HEIGHT = 1100;

    //Send geometry to the renderer
    void displayScene();

    void slowDown();
    void speedUp();
    void changePlanet(int N);

    void togglePause();
    void resetScene();
    void initializeCamera();

private:
    RenderingEngine* renderer;

    // Level of detail of sphere (Smothness)
    const int level = 36;
    const float Δ = 2*M_PI/level;
    // (Rotation)Time speed multiplier
    float KEY = 0;
    
    bool isPaused = false;

    float speedChange = 0.05;

    void initializeSceneGraph();
    void initializeNode(Node &node, const char* textureFile);

    Node* centrePlanet;
    void updateCamera();
    void updateSun();
    void updateMercury();
    void updateVenus();
    void updateVenusClouds();
    void updateEarth();
    void updateEarthClouds();
    void updateMoon();
    void updateMars();
    void updateJupiter();
    void updateSaturn();
    void updateUranus();
    void updateNeptune();
    void updateSpace();

    void updateCentre(Node &centre);
    void updatePlanet(Node &planet, bool isCloud);
    void updateSatellite(Node &satellite, Node &planet);

    glm::mat4 updateBase(float revolution);

    Node sun;
    Node mercury;

    Node venus;
    Node venusClouds;

    Node earth;
    Node earthClouds;
    Node moon;

    Node mars;
    Node deimos;
    Node phobos;

    Node ceres;

    Node jupiter;
    Node saturn;
    Node uranus;
    Node neptune;
    Node pluto;

    Node haumea;
    Node makemake;
    Node eris;

    Node space;
    std::vector<Node*> objects;

    GLint modelUniformLocation;
    GLint viewUniformLocation;
    GLint projUniformLocation;

    GLint translateUniformLocation;

    GLint diffuseLocation;
    GLint cloudLocation;

    GLint distance;

    glm::mat4 view;
    glm::mat4 proj;
    //glm::mat4 model;

    // Identity matrix
    glm::mat4 identity;
};

#endif /* SCENE_H_ */
