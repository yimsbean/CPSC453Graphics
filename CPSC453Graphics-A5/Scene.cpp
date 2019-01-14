/*
 * Scene.cpp
 *
 *  Created on: Sep 10, 2018
 *  Author: John Hall
 *
 */

#include "Scene.h"


Scene::Scene(RenderingEngine* renderer) : renderer(renderer) {
    glUseProgram(renderer->shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    if(renderer->CheckGLErrors()) {
        std::cout << "Texture creation failed" << std::endl;
    }
    initializeSceneGraph();
}
Scene::~Scene() {

}

/**
 * Called every frame in loop
 */
void Scene::displayScene() {
    /* From Boilerplate
        GLFWwindow* window; in Scene.h
        float θ; in Scene.h, 0.f default value
    */
    // Clear once
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glUseProgram(renderer->shaderProgram);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //Camera
    updateCamera();
    // Objects
    updateCentre(sun);
    updatePlanet(mercury, false);
    updatePlanet(venus, false);
    updatePlanet(venusClouds, true);
    updatePlanet(earth, false);
    updatePlanet(earthClouds, true);
    updateSatellite(moon, earth);
    updatePlanet(mars, false);
    updateSatellite(deimos, mars);
    updateSatellite(phobos, mars);
    updatePlanet(ceres, false);
    updatePlanet(jupiter, false);
    updatePlanet(saturn, false);
    updatePlanet(uranus, false);
    updatePlanet(neptune, false);
    updatePlanet(pluto, false);
    updatePlanet(haumea, false);
    updatePlanet(makemake, false);
    updatePlanet(eris, false);
    updateCentre(space);
}

/**
 * Update camera based on mouse/scrolling
 */
void Scene::updateCamera() {
    // Befor we apply the zoom
    Camera::baseLocation = glm::vec3(   sin(Camera::φ) * sin(Camera::θ),
                                        cos(Camera::θ),
                                        cos(Camera::φ) * sin(Camera::θ)
                                    );
    Camera::modifiedLocation =  Camera::zoom * Camera::baseLocation;
    Camera::direction        = -Camera::modifiedLocation;
    // Camera::direction = Camera::modifiedLocation;
    Camera::up = glm::cross(
                    glm::vec3( cos(Camera::φ), 0, -sin(Camera::φ) ),
                    Camera::direction
                );

    // MVP
    view = glm::lookAt( Camera::modifiedLocation,
                        -Camera::modifiedLocation,
                        Camera::up);
    proj = glm::perspective(Camera::fieldOfView,
                            Camera::aspectRatio,
                            Camera::zNear,
                            Camera::zFar);

    glUniformMatrix4fv(viewUniformLocation, 1, false, value_ptr(view));
    glUniformMatrix4fv(projUniformLocation, 1, false, value_ptr(proj));
}
/**
 * @param centre node that does not revolve around anything
 */
void Scene::updateCentre(Node &centre) {
    // rotates about the y axis
    centre.rotate = glm::rotate(identity, centre.rotation, glm::vec3(0, 1, 0));
    // Does not translate, in centre of scene
    centre.translate = glm::translate(identity, glm::vec3(0, 0, 0));
    centre.transform = centre.translate * centre.rotate;

    if (!isPaused) {
        centre.rotation += centre.rotationSpeed * KEY;
    }

    glUseProgram(renderer->shaderProgram);
    glUniform1ui(diffuseLocation, 0);
    glUniform1ui(cloudLocation, 0);
    glUniformMatrix4fv(modelUniformLocation, 1, false, value_ptr(centre.transform));
    glUniformMatrix4fv(translateUniformLocation,1, false, value_ptr(
        glm::translate(identity,-centrePlanet->position)
    ));
    glBindTexture(GL_TEXTURE_2D, centre.texture.textureID);
    renderer->RenderScene(centre.geometry, Camera::θ, WIDTH, HEIGHT, false);
}

/**
 * @param planet revolving around the sun
 */
void Scene::updatePlanet(Node &planet, bool isCloud) {
    // rotates about the y axis
    planet.rotate = glm::rotate(identity,
                                planet.rotation,
                                glm::vec3(0, 1, 0))
                    * planet.axis;
    // Does not translate, in centre of scene
    planet.translate = glm::translate(identity, planet.distance);
    planet.transform = sun.translate * (glm::rotate(identity, sun.rotation + planet.revolution, glm::vec3(0, 1, 0)))
                                    * glm::rotate(identity,
                                                planet.revolutionSpeed * KEY,
                                                glm::vec3(0, 1, 0))
                                   * planet.translate
                                   * planet.rotate;
    planet.position = sun.translate* (glm::rotate(identity, sun.rotation + planet.revolution, glm::vec3(0, 1, 0)))
                                   * planet.translate
                    * glm::vec4(1,1,1,1);
    if (!isPaused) {
        planet.rotation   += planet.rotationSpeed   * KEY;
        planet.revolution += planet.revolutionSpeed * KEY;
    }

    glUseProgram(renderer->shaderProgram);
    glUniform1ui(diffuseLocation, 1);
    glUniform1ui(cloudLocation, isCloud ? 1 : 0);
    glUniform1f(distance, earth.distance.z/planet.distance.z);
    glUniformMatrix4fv(modelUniformLocation, 1, false,
                        value_ptr(planet.transform));
    glUniformMatrix4fv(translateUniformLocation,1,false, value_ptr(
         glm::translate(identity,-centrePlanet->position)
    ));
    glBindTexture(GL_TEXTURE_2D, planet.texture.textureID);
    renderer->RenderScene(planet.geometry, Camera::θ, WIDTH, HEIGHT, isCloud);
}

/**
 * @param satellite revolving around a planet
 * @param planet    revolving around the sun
 */
void Scene::updateSatellite(Node &satellite, Node &planet) {
    // rotates about the y axis
    satellite.rotate = glm::rotate(identity, satellite.rotation, glm::vec3(0, 1, 0));
    // Does not translate, in centre of scene
    satellite.translate = glm::translate(identity, satellite.distance);
    satellite.transform = sun.translate * (glm::rotate(identity, sun.rotation + planet.revolution, glm::vec3(0, 1, 0)))
                                    * glm::rotate(identity,
                                                 KEY * planet.revolutionSpeed,
                                                 glm::vec3(0, 1, 0))
                                   * planet.translate
                                   * planet.axis
                                   * satellite.rotate
                                   * satellite.translate;

    satellite.position = sun.translate * (glm::rotate(identity, sun.rotation +planet.revolution, glm::vec3(0, 1, 0)))
                                   * planet.translate
                                   * planet.axis
                                   * satellite.translate
                                   * glm::vec4(1,1,1,1);
    if (!isPaused) {
        satellite.rotation += satellite.rotationSpeed * KEY;
    }

    glUseProgram(renderer->shaderProgram);
    glUniform1ui(diffuseLocation, 1);
    glUniform1ui(cloudLocation, 0);
    glUniform1f(distance, earth.distance.z/planet.distance.z);
    glUniformMatrix4fv(modelUniformLocation, 1, false, value_ptr(satellite.transform));
    glUniformMatrix4fv(translateUniformLocation,1,false, value_ptr(
        glm::translate(identity,-centrePlanet->position))
    );
    glBindTexture(GL_TEXTURE_2D, satellite.texture.textureID);
    renderer->RenderScene(satellite.geometry, Camera::θ, WIDTH, HEIGHT, false);
}

void Scene::initializeSceneGraph() {
    objects.clear();
    objects = {
        &sun,
        &mercury,
        &venus, &venusClouds,
        &earth, &earthClouds, &moon,
        &mars, &deimos, &phobos,
        &ceres,
        &jupiter,
        &saturn,
        &uranus,
        &neptune,
        &pluto,
        &haumea,
        &makemake,
        &eris,
        &space
    };

    // radii
    // TODO: What determines the size of these? Arbitrary? Some log operation?
    // If arbitrary, moon may be too big
    // Mars is twice as big as moon IRL
    float logbase = 8*log(1.2);
    sun.radius          = log(695.508)/logbase;
    mercury.radius      = log(2.4397)/logbase;     //log1.2(X)/8, X=2.5k
    venus.radius        = log(6.3782)/logbase;     //log1.2(X)/8, X=6K
    venusClouds.radius  = venus.radius + 0.01;
    earth.radius        = log(6.0)/logbase;     //log1.2(X)/8, X=6K
    earthClouds.radius  = earth.radius + 0.01;
    // moon.radius         = 0.3;
    moon.radius         = log(1.737)/logbase;
    mars.radius         = log(3.3962)/logbase;     //log1.2(X)/8, X=3.5K
    deimos.radius       = 0.2;
    phobos.radius       = 0.2;
    ceres.radius        = 0.2;
    jupiter.radius      = log(71.495)/logbase;      //log1.2(X)/8, X=71K
    saturn.radius       = log(60.268)/logbase;      //log1.2(X)/8, X=60K
    uranus.radius       = log(25.559)/logbase;      //log1.2(X)/8, X=25K
    neptune.radius      = log(24.764)/logbase;      //log1.2(X)/8, X=24K
    // pluto.radius        = log(1.195)/logbase;      //log1.2(X)/8, X=1K
    pluto.radius        = 0.2;
    haumea.radius       = 0.2;
    makemake.radius     = 0.2;
    eris.radius         = 0.2;

     // Revolution distance for orbit
     // x = azimuth from starting line at start of animation, but not radial, so
     //     it actually increases z distance
     // y = angle deviation from orbital plane
     // z = distance from parent
    sun.distance         = glm::vec3(0, 10*sin(Math::degreesToRadians(0)),     0);
    mercury.distance     = glm::vec3(0, 10*sin(Math::degreesToRadians(-6.3)),   60*log10(1+0.38)); //30log10(X+1), X=0.38AU
    venus.distance       = glm::vec3(0, 10*sin(Math::degreesToRadians(2.2)),  60*log10(1+0.72)); //30log10(X+1), X=0.72AU
    venusClouds.distance = venus.distance;
    earth.distance       = glm::vec3(0, 10*sin(Math::degreesToRadians(1.6)),  60*log10(1+1));  //30log10(X+1), X=1AU
    earthClouds.distance = earth.distance;
    moon.distance        = glm::vec3(0, 10*sin(Math::degreesToRadians(0)),     2);
    mars.distance        = glm::vec3(0, 10*sin(Math::degreesToRadians(1.7)),  60*log10(1+1.52)); //30log10(X+1), X=1.52AU
    deimos.distance      = glm::vec3(0, 10*sin(Math::degreesToRadians(0)),     1);
    phobos.distance      = glm::vec3(0, 10*sin(Math::degreesToRadians(0)),     2);
    ceres.distance       = glm::vec3(0, 10*sin(Math::degreesToRadians(-10.59)), 60*log10(1+2.77));
    jupiter.distance     = glm::vec3(0, 10*sin(Math::degreesToRadians(0.3)),  60*log10(1+5.2)); //30log10(X+1), X=5.2AU
    saturn.distance      = glm::vec3(0, 10*sin(Math::degreesToRadians(0.9)),  60*log10(1+9.58)); //30log10(X+1), X=9.58AU
    uranus.distance      = glm::vec3(0, 10*sin(Math::degreesToRadians(1.0)),  60*log10(1+19.19)); //30log10(X+1), X=19.19AU
    neptune.distance     = glm::vec3(0, 10*sin(Math::degreesToRadians(0.7)),  60*log10(1+30.2)); //30log10(X+1), X=30.2AU
    pluto.distance       = glm::vec3(0, 10*sin(Math::degreesToRadians(17.0)), 60*log10(1+39.53)); //30log10(X+1), X=39.53AU
    haumea.distance      = glm::vec3(0, 10*sin(Math::degreesToRadians(-28.2)),  60*log10(1+43.1));
    makemake.distance    = glm::vec3(0, 10*sin(Math::degreesToRadians(-29)),    60*log10(1+45.8));
    eris.distance        = glm::vec3(0, 10*sin(Math::degreesToRadians(-46.9)),  60*log10(1+96.4));
    space.distance       = glm::vec3(0, 10*sin(Math::degreesToRadians(0)),     0);

    space.radius        = Camera::MAX_ZOOM + 100 + eris.distance.z;

    // Axial tilts from Wikipedia
    mercury.axis = glm::rotate(identity,
                               Math::degreesToRadians(0.3),
                               glm::vec3(1, 0, 0));
    venus.axis = glm::rotate(identity,
                             Math::degreesToRadians(2.64),
                             glm::vec3(1, 0, 0));
    earth.axis = glm::rotate(identity,
                             Math::degreesToRadians(23.44),
                             glm::vec3(1, 0, 0));
    mars.axis = glm::rotate(identity,
                            Math::degreesToRadians(25.19),
                            glm::vec3(1, 0, 0));
    ceres.axis = glm::rotate(identity,
                            Math::degreesToRadians(4),
                            glm::vec3(1, 0, 0));
    jupiter.axis = glm::rotate(identity,
                               Math::degreesToRadians(3.13),
                               glm::vec3(1, 0, 0));
    saturn.axis = glm::rotate(identity,
                              Math::degreesToRadians(26.73),
                              glm::vec3(1, 0, 0));
    uranus.axis = glm::rotate(identity,
                              Math::degreesToRadians(82.23),
                              glm::vec3(1, 0, 0));
    neptune.axis = glm::rotate(identity,
                               Math::degreesToRadians(28.32),
                               glm::vec3(1, 0, 0));
    pluto.axis = glm::rotate(identity,
                             Math::degreesToRadians(119.61),
                             glm::vec3(1, 0, 0));
    haumea.axis = glm::rotate(identity,
                             Math::degreesToRadians(0), // unknown
                             glm::vec3(1, 0, 0));
    makemake.axis = glm::rotate(identity,
                             Math::degreesToRadians(0), //unknown
                             glm::vec3(1, 0, 0));
    eris.axis = glm::rotate(identity,
                             Math::degreesToRadians(0), //unknown
                             glm::vec3(1, 0, 0));


    // rotation speed around axis
    float baseAngle = 15; //earth
    float baseDate = 24;

    // sun.rotationSpeedBase         = Math::degreesToRadians(0.4);
    sun.rotationSpeedBase         = Math::degreesToRadians(baseAngle * baseDate / 587.28);      // 24.47 days
    mercury.rotationSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 1407.6);      // 1407.6 hours
    venus.rotationSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / -5832.5);     //  -5832.5 hours
    earth.rotationSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / baseDate);    // 24 hours
    earthClouds.rotationSpeedBase = Math::degreesToRadians(40);                                // arbitrarily faster than earth
    moon.rotationSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 648);        //  655.7 hours ?
    mars.rotationSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 24.6);       //  24.6 hours
    deimos.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 30);        //
    phobos.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 8);         //
    ceres.rotationSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / 9);         // 9 hours
    jupiter.rotationSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 9.9);       //  9.9 hours
    saturn.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 10.7);      //  10.7 hours
    uranus.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / -17.2);     //  -17.2 hours
    neptune.rotationSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 16.1);      //  16.1 hours
    pluto.rotationSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / 153.3);      //  153.3 hours
    haumea.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 3.9);      //  3.9 hours
    makemake.rotationSpeedBase    = Math::degreesToRadians(baseAngle * baseDate / 22.5);      //  22.5 hours
    eris.rotationSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 25.9);      //  25.9 hours
    space.rotationSpeedBase       = Math::degreesToRadians(0);

    // Revolution speed for orbit
    baseAngle = 0.2;
    baseDate = 365;
    sun.revolutionSpeedBase         = 0; //Not in use
    mercury.revolutionSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 88);     // 88 days
    venus.revolutionSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / 224.7);     // 224.7 days
    venusClouds.revolutionSpeedBase = venus.revolutionSpeedBase;
    earth.revolutionSpeedBase       = Math::degreesToRadians(baseAngle);     // 365.2 days
    earthClouds.revolutionSpeedBase = earth.revolutionSpeedBase;
    moon.revolutionSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 27.3);    // 27.3 days
    mars.revolutionSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 687);     // 687 days
    deimos.revolutionSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 30);
    phobos.revolutionSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 8);
    ceres.rotationSpeedBase         = Math::degreesToRadians(baseAngle * baseDate / 40320);      // 1680 days
    jupiter.revolutionSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 4431);     // 4331 days
    saturn.revolutionSpeedBase      = Math::degreesToRadians(baseAngle * baseDate /10747);     // 10747 days
    uranus.revolutionSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 30589);      // 30589 days
    neptune.revolutionSpeedBase     = Math::degreesToRadians(baseAngle * baseDate / 59800);      // 59800 days
    pluto.revolutionSpeedBase       = Math::degreesToRadians(baseAngle * baseDate / 90520);      // 59800 days
    haumea.rotationSpeedBase        = Math::degreesToRadians(baseAngle * baseDate / 2172480);      //  248 years
    makemake.rotationSpeedBase      = Math::degreesToRadians(baseAngle * baseDate / 2706840);      //  309 years
    eris.rotationSpeedBase          = Math::degreesToRadians(baseAngle * baseDate / 4888080);      //  558 years
    space.revolutionSpeedBase       = 0; // TODO We can make the stars move

    initializeNode(sun,         "images/sun.jpg");
    initializeNode(mercury,     "images/mercury.jpg");
    initializeNode(venus,       "images/venus_surface.jpg");
    initializeNode(venusClouds, "images/venus_atmosphere.jpg");
    initializeNode(earth,       "images/earth_day.jpg");
    initializeNode(earthClouds, "images/clouds.jpg");
    initializeNode(moon,        "images/moon.jpg");
    initializeNode(mars,        "images/mars.jpg");
    initializeNode(deimos,      "images/deimos.jpg");
    initializeNode(phobos,      "images/phobos.jpg");
    initializeNode(ceres,      "images/ceres.jpg");
    initializeNode(jupiter,     "images/jupiter.jpg");
    initializeNode(saturn,      "images/saturn.jpg");
    initializeNode(uranus,      "images/uranus.jpg");
    initializeNode(neptune,     "images/neptune.jpg");
    initializeNode(pluto,       "images/pluto.jpg");
    initializeNode(haumea,      "images/haumea.jpg");
    initializeNode(makemake,    "images/makemake.jpg");
    initializeNode(eris,        "images/eris.jpg");
    initializeNode(space,       "images/space.jpg");

    identity = glm::mat4(1); // Identity matrix
    centrePlanet = &sun;


    // Shader variables
    diffuseLocation = glGetUniformLocation(renderer->shaderProgram, "diffuse");
    cloudLocation   = glGetUniformLocation(renderer->shaderProgram, "clouds");
    distance        = glGetUniformLocation(renderer->shaderProgram, "distance");
    // Get the locations of the MVP
    modelUniformLocation = glGetUniformLocation(renderer->shaderProgram, "model");
    viewUniformLocation  = glGetUniformLocation(renderer->shaderProgram, "view");
    projUniformLocation  = glGetUniformLocation(renderer->shaderProgram, "proj");

    translateUniformLocation = glGetUniformLocation(renderer->shaderProgram, "trans");


    resetScene();
}

void Scene::initializeCamera() {
    Camera::φ = Camera::PHI_DEFAULT;
    Camera::θ = Camera::THETA_DEFAULT;
    Camera::aspectRatio = (float)WIDTH / (float)HEIGHT;
    Camera::zoom = Camera::DEFAULT_ZOOM;
    Camera::zNear = 1.f;
    Camera::zFar = 1000.1f;
    Camera::fieldOfView = Math::degreesToRadians(Camera::FOV_DEFAULT);
}

// TODO clean
// InitializeGeometry
void Scene::initializeNode(Node &node, const char* textureFile) {
    float θ;
    float φ;

    for (int i = 0; i < node.level / 2; i++) {
        θ = i * Δ;

        for (int j = 0; j < node.level; j++) {
            φ = j * Δ;

            std::vector<glm::vec3> vert;
            std::vector<glm::vec2> uv;
            Geometry triangle;

            // Triangle 1
            vert = {
                glm::vec3(  sin(φ)     * sin(θ),
                            cos(θ),
                            cos(φ)     * sin(θ)),
                glm::vec3(  sin(φ)     * sin(θ + Δ),
                            cos(θ + Δ),
                            cos(φ)     * sin(θ + Δ)),
                glm::vec3(  sin(φ + Δ) * sin(θ + Δ),
                            cos(θ + Δ),
                            cos(φ + Δ) * sin(θ + Δ))
            };
            uv = {
                glm::vec2(  (φ)     /   (M_PI*2),
                            (θ)     /   (M_PI)),
                glm::vec2(  (φ)     /   (M_PI*2),
                            (θ + Δ) /   (M_PI)),
                glm::vec2(  (φ + Δ) /   (M_PI*2),
                            (θ + Δ) /   M_PI)
            };

            for (int i = 0; i < 3; ++i) {
                triangle.verts.push_back(vert[i] * node.radius);
                triangle.normals.push_back(vert[i]);
                triangle.uvs.push_back(uv[i]);
                triangle.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            }

            // Triangle 2
            vert = {
                glm::vec3(  sin(φ)     * sin(θ),
                            cos(θ),
                            cos(φ)     * sin(θ)),
                glm::vec3(  sin(φ + Δ) * sin(θ + Δ),
                            cos(θ + Δ),
                            cos(φ + Δ) * sin(θ + Δ)),
                glm::vec3(  sin(φ + Δ) * sin(θ),
                            cos(θ),
                            cos(φ + Δ) * sin(θ))
            };
            uv = {
                glm::vec2(  (φ)     /   (M_PI*2),
                            (θ)     /   (M_PI)),
                glm::vec2(  (φ + Δ) /   (M_PI*2),
                            (θ + Δ) /   (M_PI)),
                glm::vec2(  (φ + Δ) /   (M_PI*2),
                            (θ)     /   (M_PI))
            };
            for (int i = 0; i < 3; ++i) {
                triangle.verts.push_back(vert[i] * node.radius);
                triangle.normals.push_back(vert[i]);
                triangle.uvs.push_back(uv[i]);
                triangle.colors.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            }

            //common parts
            triangle.drawMode = GL_TRIANGLES;

            RenderingEngine::assignBuffers(triangle);
            RenderingEngine::setBufferData(triangle);
            node.geometry.push_back(triangle);
        }
    }

    if (renderer->CheckGLErrors()) {
        std::cout << "initializeNode() Failed to create geometry" << '\n';
        exit(1);
    } else {
        InitializeTexture(&node.texture, textureFile);
    }
}

void Scene::slowDown() {
    KEY -= speedChange;
}

void Scene::speedUp() {
    KEY += speedChange;
}

void Scene::togglePause() {
    isPaused = !isPaused;
}
void Scene::changePlanet(int N){
    switch(N) {
        case 0:
            centrePlanet = &sun; break;
        case 1:
            centrePlanet = &mercury; break;
        case 2:
            centrePlanet = &venus; break;
        case 3:
            centrePlanet = &earth; break;
        case 4:
            centrePlanet = &mars; break;
        case 5:
            centrePlanet = &ceres; break;
        case 6:
            centrePlanet = &jupiter; break;
        case 7:
            centrePlanet = &saturn; break;
        case 8:
            centrePlanet = &uranus; break;
        case 9:
            centrePlanet = &neptune; break;
        case 10:
            centrePlanet = &pluto; break;
        case 11:
            centrePlanet = &haumea; break;
        case 12:
            centrePlanet = &makemake; break;
        case 13:
            centrePlanet = &eris; break;
        default:
            std::cout << "HACKER" << '\n';
            exit(-1);
    }
    Camera::MIN_ZOOM = 2 * centrePlanet->radius;
    if (Camera::zoom < Camera::MIN_ZOOM) {
        Camera::zoom = Camera::MIN_ZOOM;
    }
}
void Scene::resetScene() {
    KEY = 1;
    initializeCamera();

    //initializer
    for (Node* n : objects) {
        n->rotation        = 0;                      //Sun | Earth | Cloud | Moon | Mars
        n->revolution      = 0;                      //      Earth | Cloud |      | Mars
        n->rotationSpeed   = n->rotationSpeedBase;   //Sun | Earth | Cloud | Moon | Mars
        n->revolutionSpeed = n->revolutionSpeedBase; //    | Earth | Cloud |      | Mars
        n->position        = n->distance;
    }
}

glm::mat4 Scene::updateBase(float revolution) {
    return sun.translate * (glm::rotate(identity, sun.rotation + revolution,
                                        glm::vec3(0, 1, 0))
                            );
}
