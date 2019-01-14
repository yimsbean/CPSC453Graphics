/*
 * RenderingEngine.cpp
 *
 *  Created on: Sep 10, 2018
 *      Author: John Hall
 */

#include "RenderingEngine.h"
#include "texture.h"

#include <iostream>

//cpp file purposely included here because it just contains some global functions
#include "ShaderTools.h"

RenderingEngine::RenderingEngine() {
    shaderProgram = ShaderTools::InitializeShaders();
    if (shaderProgram == 0) {
        std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
        return;
    }

}

RenderingEngine::~RenderingEngine() {

}

void RenderingEngine::RenderScene(const std::vector<Geometry>& objects, float theta, int width, int height, bool blend) {
    //Clears the screen to a dark grey background
    //glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    if (blend) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

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

    const int VERTEX_INDEX  = 0;
    const int COLOR_INDEX   = 1;
    const int TEXTURE_INDEX = 2;
    const int NORMAL_INDEX  = 3;

    //Generate vao for the object
    //Constant 1 means 1 vao is being generated
    glGenVertexArrays(1, &geometry.vao);
    glBindVertexArray(geometry.vao);

    // VBOS - 0
    //Generate vbos for the object
    //Constant 1 means 1 vbo is being generated
    glGenBuffers(1, &geometry.vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
    //Parameters in order: Index of vbo in the vao, number of primitives per
    //element, primitive type, etc.
    glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(VERTEX_INDEX);

    // NORMALS - 3
    glGenBuffers(1, &geometry.normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
    glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(NORMAL_INDEX);

    // COLORS - 1
    glGenBuffers(1, &geometry.colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
    //Parameters in order: Index of vbo in the vao, number of primitives per element, primitive type, etc.
    glVertexAttribPointer(COLOR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(COLOR_INDEX);

    // UV - 2
    glGenBuffers(1, &geometry.uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry.uvBuffer);
    glVertexAttribPointer(TEXTURE_INDEX, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(TEXTURE_INDEX);


}

void RenderingEngine::setBufferData(Geometry& geometry) {
    //Send geometry to the GPU
    //Must be called whenever anything is updated about the object
    glBindBuffer(GL_ARRAY_BUFFER, geometry.vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.verts.size(), geometry.verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, geometry.normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.normals.size(), geometry.normals.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, geometry.colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * geometry.colors.size(), geometry.colors.data(), GL_STATIC_DRAW);

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
    for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError()) {
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
