// ==========================================================================
// Barebones OpenGL Core Profile Boilerplate
//    using the GLFW windowing system (http://www.glfw.org)
//
// Loosely based on
//  - Chris Wellons' example (https://github.com/skeeto/opengl-demo) and
//  - Camilla Berglund's example (http://www.glfw.org/docs/latest/quick.html)
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>
#include <glad/glad.h>
// Specify that we want the OpenGL core profile before including GLFW headers
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
// --------------------------------------------------------------------------
// OpenGL utility and support function prototypes

void QueryGLVersion();
bool CheckGLErrors();

std::string LoadSource(const std::string &filename);
GLuint CompileShader(GLenum shaderType, const std::string &source);
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);

// --------------------------------------------------------------------------
// Functions to set up OpenGL shader programs for rendering

struct MyShader
{
    // OpenGL names for vertex and fragment shaders, shader program
    GLuint  vertex;
    GLuint  fragment;
    GLuint  program;

    // initialize shader and program names to zero (OpenGL reserved value)
    MyShader() : vertex(0), fragment(0), program(0)
    {}
};

struct Coordinate
{
    double x;
    double y;
};

struct MouseStatus 
{
    bool button_pressed;
    Coordinate location_offset;
    Coordinate prev_location_offset;
    Coordinate mouse_press;
    float zoom;
};
MouseStatus mouse_status;

struct KeyboardStatus 
{
    bool paused = false;
};
KeyboardStatus keyboard_status;

float earthrotation = 0.0;
float cloudsrotation = 0.0;

float sunrotation_b = 0.00695;
float earthrotation_b = 0.4;
float earth_rotation_b = 0.005;
float cloudsrotation_b = 0.7;
float clouds_rotation_b = 0.005;
float moonrotation_b = 0.12;
// load, compile, and link shaders, returning true if successful
bool InitializeShaders(MyShader *shader)
{
    // load shader source from files
    std::string vertexSource = LoadSource("vertex.glsl");
    std::string fragmentSource = LoadSource("fragment.glsl");
    if (vertexSource.empty() || fragmentSource.empty()) return false;

    // compile shader source into shader objects
    shader->vertex = CompileShader(GL_VERTEX_SHADER, vertexSource);
    shader->fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    // link shader program
    shader->program = LinkProgram(shader->vertex, shader->fragment);

    // check for OpenGL errors and return false if error occurred
    return !CheckGLErrors();
}

// deallocate shader-related objects
void DestroyShaders(MyShader *shader)
{
    // unbind any shader programs and destroy shader objects
    glUseProgram(0);
    glDeleteProgram(shader->program);
    glDeleteShader(shader->vertex);
    glDeleteShader(shader->fragment);
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing textures

struct MyTexture
{
    GLuint textureID;
    GLuint target;
    int width;
    int height;

    // initialize object names to zero (OpenGL reserved value)
    MyTexture() : textureID(0), target(0), width(0), height(0)
    {}
};

bool InitializeTexture(MyTexture* texture, const char* filename, GLuint target = GL_TEXTURE_2D)
{
    int numComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filename, &texture->width, &texture->height, &numComponents, 0);
    if (data != nullptr)
    {
        texture->target = target;
        glGenTextures(1, &texture->textureID);
        glBindTexture(texture->target, texture->textureID);
        GLuint format = numComponents == 3 ? GL_RGB : GL_RGBA;
        glTexImage2D(texture->target, 0, format, texture->width, texture->height, 0, format, GL_UNSIGNED_BYTE, data);

        // Note: Only wrapping modes supported for GL_TEXTURE_RECTANGLE when defining
        // GL_TEXTURE_WRAP are GL_CLAMP_TO_EDGE or GL_CLAMP_TO_BORDER
        glTexParameteri(texture->target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture->target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(texture->target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(texture->target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Clean up
        glBindTexture(texture->target, 0);
        stbi_image_free(data);
        return !CheckGLErrors();
    }
    return true; //error
}

// deallocate texture-related objects
void DestroyTexture(MyTexture *texture)
{
    glBindTexture(texture->target, 0);
    glDeleteTextures(1, &texture->textureID);
}

// --------------------------------------------------------------------------
// Functions to set up OpenGL buffers for storing geometry data

struct MyGeometry
{
    // OpenGL names for array buffer objects, vertex array object
    GLuint  vertexBuffer;
    GLuint  normalBuffer;
    GLuint  textureBuffer;
    GLuint  colourBuffer;
    GLuint  elementBuffer;
    GLuint  vertexArray;
    GLsizei elementCount;

    // initialize object names to zero (OpenGL reserved value)
    MyGeometry() : vertexBuffer(0),
                   normalBuffer(0),
                   textureBuffer(0),
                   colourBuffer(0),
                   vertexArray(0),
                   elementCount(0)
    {}
};

void initialize_sphere(std::vector<GLfloat>& vertices, std::vector<GLfloat>& normals, std::vector<GLfloat>& textureCoords, std::vector<GLfloat>& colours, float radius, float level)
{
    float degree = 2*M_PI/level;
    float theta, phi, next_theta, next_phi;

    for (int i = 0; i < level/2; ++i) {
        theta = i*degree;
        next_theta = theta + degree;

        for (int j = 0; j < level; ++j) {
            phi = j*degree;
            next_phi = phi + degree;

            // first triangle
            // top left, bottom left, bottom right
            float triangle1_x1 = sin(phi)*sin(theta);
            float triangle1_y1 = cos(theta);
            float triangle1_z1 = cos(phi)*sin(theta);

            float triangle1_x2 = sin(phi)*sin(next_theta);
            float triangle1_y2 = cos(next_theta);
            float triangle1_z2 = cos(phi)*sin(next_theta);

            float triangle1_x3 = sin(next_phi)*sin(next_theta);
            float triangle1_y3 = cos(next_theta);
            float triangle1_z3 = cos(next_phi)*sin(next_theta);

            vertices.push_back(radius*triangle1_x1);
            vertices.push_back(radius*triangle1_y1);
            vertices.push_back(radius*triangle1_z1);
            vertices.push_back(radius*triangle1_x2);
            vertices.push_back(radius*triangle1_y2);
            vertices.push_back(radius*triangle1_z2);
            vertices.push_back(radius*triangle1_x3);
            vertices.push_back(radius*triangle1_y3);
            vertices.push_back(radius*triangle1_z3);

            normals.push_back(triangle1_x1);
            normals.push_back(triangle1_y1);
            normals.push_back(triangle1_z1);
            normals.push_back(triangle1_x2);
            normals.push_back(triangle1_y2);
            normals.push_back(triangle1_z2);
            normals.push_back(triangle1_x3);
            normals.push_back(triangle1_y3);
            normals.push_back(triangle1_z3);

            textureCoords.push_back(float(phi/(M_PI*2)));
            textureCoords.push_back(float(theta/M_PI));

            textureCoords.push_back(float(phi/(M_PI*2)));
            textureCoords.push_back(float(next_theta/M_PI));

            textureCoords.push_back(float(next_phi/(M_PI*2)));
            textureCoords.push_back(float(next_theta/M_PI));

            // second triangle
            // top left, bottom right, top right
            float triangle2_x1 = sin(phi)*sin(theta);
            float triangle2_y1 = cos(theta);
            float triangle2_z1 = cos(phi)*sin(theta);
            float triangle2_x2 = sin(next_phi)*sin(next_theta);
            float triangle2_y2 = cos(next_theta);
            float triangle2_z2 = cos(next_phi)*sin(next_theta);
            float triangle2_x3 = sin(next_phi)*sin(theta);
            float triangle2_y3 = cos(theta);
            float triangle2_z3 = cos(next_phi)*sin(theta);

            vertices.push_back(radius*triangle2_x1);
            vertices.push_back(radius*triangle2_y1);
            vertices.push_back(radius*triangle2_z1);
            vertices.push_back(radius*triangle2_x2);
            vertices.push_back(radius*triangle2_y2);
            vertices.push_back(radius*triangle2_z2);
            vertices.push_back(radius*triangle2_x3);
            vertices.push_back(radius*triangle2_y3);
            vertices.push_back(radius*triangle2_z3);

            normals.push_back(triangle2_x1);
            normals.push_back(triangle2_y1);
            normals.push_back(triangle2_z1);
            normals.push_back(triangle2_x2);
            normals.push_back(triangle2_y2);
            normals.push_back(triangle2_z2);
            normals.push_back(triangle2_x3);
            normals.push_back(triangle2_y3);
            normals.push_back(triangle2_z3);

            textureCoords.push_back(float(phi/(M_PI*2)));
            textureCoords.push_back(float(theta/M_PI));
            textureCoords.push_back(float(next_phi/(M_PI*2)));
            textureCoords.push_back(float(next_theta/M_PI));
            textureCoords.push_back(float(next_phi/(M_PI*2)));
            textureCoords.push_back(float(theta/M_PI));
        }
    }

    for (uint i = 0; i <= vertices.size(); i++) {
        colours.push_back(1);
        colours.push_back(1);
        colours.push_back(1);
    }
}

// create buffers and fill with geometry data, returning true if successful
bool InitializeGeometry(MyGeometry *geometry, float radius, float level)
{
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textureCoords;
    std::vector<GLfloat> colours;
    initialize_sphere(vertices, normals, textureCoords, colours, radius, level);

    // make it a white sphere

    geometry->elementCount = vertices.size();

    const GLuint VERTEX_INDEX = 0;
    const GLuint COLOUR_INDEX = 1;
    const GLuint TEXTURE_INDEX = 2;
    const GLuint NORMAL_INDEX = 3;

    // create an array buffer object for storing our vertices
    glGenBuffers(1, &geometry->vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

    // create another one for storing our colours
    glGenBuffers(1, &geometry->colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
    glBufferData(GL_ARRAY_BUFFER, colours.size() * sizeof(GLfloat), &colours[0], GL_STATIC_DRAW);

    // create another one for storing our textures
    glGenBuffers(1, &geometry->textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
    glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), &textureCoords[0], GL_STATIC_DRAW);

    // create another one for storing our normals
    glGenBuffers(1, &geometry->normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), &normals[0], GL_STATIC_DRAW);

    // create a vertex array object encapsulating all our vertex attributes
    glGenVertexArrays(1, &geometry->vertexArray);
    glBindVertexArray(geometry->vertexArray);

    // create another one for storing our indices into the vertices
    //glGenBuffers(1, &geometry->elementBuffer);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->elementBuffer);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->textureBuffer);
    glVertexAttribPointer(TEXTURE_INDEX, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(TEXTURE_INDEX);

    glBindBuffer(GL_ARRAY_BUFFER, geometry->normalBuffer);
    glVertexAttribPointer(NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(NORMAL_INDEX);

    // associate the position array with the vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, geometry->vertexBuffer);
    glVertexAttribPointer(VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(VERTEX_INDEX);

    // assocaite the colour array with the vertex array object
    glBindBuffer(GL_ARRAY_BUFFER, geometry->colourBuffer);
    glVertexAttribPointer(COLOUR_INDEX, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(COLOUR_INDEX);

    // unbind our buffers, resetting to default state
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // check for OpenGL errors and return false if error occurred
    return !CheckGLErrors();
}

// deallocate geometry-related objects
void DestroyGeometry(MyGeometry *geometry)
{
    // unbind and destroy our vertex array object and associated buffers
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &geometry->vertexArray);
    glDeleteBuffers(1, &geometry->vertexBuffer);
    glDeleteBuffers(1, &geometry->colourBuffer);
    glDeleteBuffers(1, &geometry->elementBuffer);
}

// --------------------------------------------------------------------------
// Rendering function that draws our scene to the frame buffer

void RenderScene(MyGeometry *geometry, MyShader *shader, MyTexture *texture, bool clouds)
{
    // clear screen to a black colour
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    if(clouds) {
        glEnable (GL_BLEND); 
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
    }
    /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

    // bind our shader program and the vertex array object containing our
    // scene geometry, then tell OpenGL to draw our geometry
    glBindTexture(texture->target, texture->textureID);
    glUseProgram(shader->program);
    glBindVertexArray(geometry->vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, geometry->elementCount);

    // reset state to default (no shader or geometry bound)
    glBindTexture(texture->target, 0);
    glBindVertexArray(0);
    glUseProgram(0);

    // check for an report any OpenGL errors
    CheckGLErrors();
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        mouse_status.button_pressed = true;
        glfwGetCursorPos(window, &mouse_status.mouse_press.x, &mouse_status.mouse_press.y);
        mouse_status.prev_location_offset = mouse_status.location_offset;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        mouse_status.button_pressed = false;
    }
}

static void cursor_callback(GLFWwindow* window, double xpos, double ypos) {
    if(mouse_status.button_pressed) {
        mouse_status.location_offset.x = mouse_status.prev_location_offset.x - ((mouse_status.mouse_press.x - xpos))/100;
        mouse_status.location_offset.y = mouse_status.prev_location_offset.y + ((mouse_status.mouse_press.y - ypos))/100;
    } 
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if((mouse_status.zoom >= 18) && (mouse_status.zoom <= 40)) {
        mouse_status.zoom -= yoffset/10;
    } else if(mouse_status.zoom < 18) {
        mouse_status.zoom = 18;
    } else if(mouse_status.zoom > 40) {
        mouse_status.zoom = 39;
    }
}

struct Planet 
{
    Planet* orbit;
    MyGeometry geometry;
    MyTexture texture;
    glm::mat4 rotate;
    glm::mat4 translate;
    glm::mat4 transform;
    float rotation = 0;
};

Planet sun;
Planet moon;
Planet earth;
Planet space;
Planet clouds;
// --------------------------------------------------------------------------
// GLFW callback functions

// reports GLFW errors
void ErrorCallback(int error, const char* description)
{
    std::cout << "GLFW ERROR " << error << ":" << std::endl;
    std::cout << description << std::endl;
}

// handles keyboard input events
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (keyboard_status.paused) {
            keyboard_status.paused = false;
        } else if (!keyboard_status.paused) {
            keyboard_status.paused = true;
        }
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        mouse_status.location_offset.x = -0.4;
        mouse_status.location_offset.y = 4.36;
        mouse_status.zoom = 18;
        moon.rotation = 0;
        sun.rotation = 0;
        earth.rotation = 0;
        clouds.rotation = 0;
        earthrotation = 0;
        cloudsrotation = 0;

        sunrotation_b = 0.00695;
        earthrotation_b = 0.4;
        earth_rotation_b = 0.005;
        cloudsrotation_b = 0.7;
        clouds_rotation_b = 0.005;
        moonrotation_b = 0.12;
    }
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        sunrotation_b /= 2;
        earthrotation_b /= 2;
        earth_rotation_b /=2;
        cloudsrotation_b /=2;
        clouds_rotation_b /=2;
        moonrotation_b /= 2;
    }
    if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
        sunrotation_b *= 2;
        earthrotation_b *= 2;
        earth_rotation_b *=2;
        cloudsrotation_b *=2;
        clouds_rotation_b *=2;
        moonrotation_b *= 2;
    }
}

// ==========================================================================
// PROGRAM ENTRY POINT
int main(int argc, char *argv[])
{
    // initialize the GLFW windowing system
    if (!glfwInit()) {
        std::cout << "ERROR: GLFW failed to initialize, TERMINATING" << std::endl;
        return -1;
    }
    
    glfwSetErrorCallback(ErrorCallback);
    mouse_status.location_offset.x = -0.4;
    mouse_status.location_offset.y = 4.36;
    mouse_status.zoom = 18;

    // attempt to create a window with an OpenGL 4.1 core profile context
    GLFWwindow *window = 0;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    int width = 1000, height = 1000;
    window = glfwCreateWindow(width, height, "CPSC 453 OpenGL Boilerplate", 0, 0);
    if (!window) {
        std::cout << "Program failed to create GLFW window, TERMINATING" << std::endl;
        glfwTerminate();
        return -1;
    }

    // set keyboard callback function and make our context current (active)
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, mouse_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwMakeContextCurrent(window);

    //Intialize GLAD
    if (!gladLoadGL())
    {
        std::cout << "GLAD init failed" << std::endl;
        return -1;
    }
    std::cout<<"TEST"<<std::endl;
    // query and print out information about our OpenGL environment
    QueryGLVersion();

    // call function to load and compile shader programs
    MyShader shader;
    if (!InitializeShaders(&shader)) {
        std::cout << "Program could not initialize shaders, TERMINATING" << std::endl;
        return -1;
    }

    // call function to create and fill buffers with geometry data
    
    if (!InitializeGeometry(&earth.geometry, 1.5, 36))
        std::cout << "Program failed to intialize geometry!" << std::endl;
    InitializeTexture(&earth.texture, "images/earth.jpg");
    if (!InitializeGeometry(&sun.geometry, 3, 36))
        std::cout << "Program failed to intialize geometry!" << std::endl;
    InitializeTexture(&sun.texture, "images/sun.jpg");
    if (!InitializeGeometry(&moon.geometry, 0.8, 36))
        std::cout << "Program failed to intialize geometry!" << std::endl;
    InitializeTexture(&moon.texture, "images/moon.jpg");
    if (!InitializeGeometry(&space.geometry, 40, 36))
        std::cout << "Program failed to intialize geometry!" << std::endl;
    InitializeTexture(&space.texture, "images/space.jpg");

    // Bonus
    if (!InitializeGeometry(&clouds.geometry, 1.6, 36))
        std::cout << "Program failed to intialize geometry!" << std::endl;
    InitializeTexture(&clouds.texture, "images/clouds.jpg");

    glm::vec3 location(0,0,0);
    glm::vec3 axis(0,1,0);

    float camera_phi = 0;
    float camera_theta = 0;

    float aspectRatio = (float)width/ (float)height;
    float zNear = .1f, zFar = 1000.f;
    float fov = 1.0472f;

    glm::mat4 I(1);
    glUseProgram(shader.program);
    GLint modelUniform = glGetUniformLocation(shader.program, "model");
    GLint viewUniform = glGetUniformLocation(shader.program, "view");
    GLint projUniform = glGetUniformLocation(shader.program, "proj");
    // run an event-triggered main loop

    while (!glfwWindowShouldClose(window))
    {
        glUseProgram(shader.program);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera_phi = mouse_status.location_offset.x;
        camera_theta = mouse_status.location_offset.y;

        glm::vec3 cameraLoc( sin(camera_phi)*sin(camera_theta), cos(camera_theta), cos(camera_phi)*sin(camera_theta));
        glm::vec3 modified_camera_loc = mouse_status.zoom * cameraLoc;
        glm::vec3 cameraDir = -modified_camera_loc;
        glm::vec3 cameraUp = glm::cross(glm::vec3( cos(camera_phi), 0, -sin(camera_phi)), cameraDir);

        glm::mat4 view = glm::lookAt(modified_camera_loc, -modified_camera_loc, cameraUp);
        glm::mat4 proj = glm::perspective(fov, aspectRatio, zNear, zFar);
        glUniformMatrix4fv(viewUniform, 1, false, value_ptr(view));
        glUniformMatrix4fv(projUniform, 1, false, value_ptr(proj));

        // Sun
        sun.rotate = glm::rotate(I, sun.rotation, axis); 
        sun.translate = glm::translate(I, glm::vec3(0,0,0));
        glm::mat4 model = sun.translate * sun.rotate;
        sun.transform = model;
        if(!keyboard_status.paused)
            sun.rotation += sunrotation_b;

        glUseProgram(shader.program);
        GLint diffuse = glGetUniformLocation(shader.program, "diffuse");
        glUniform1ui(diffuse, 0);
        GLint cloud = glGetUniformLocation(shader.program, "clouds");
        glUniform1ui(cloud, 0);
        glUniformMatrix4fv(modelUniform, 1, false, value_ptr(model));
        RenderScene(&sun.geometry, &shader, &sun.texture, false);

        // Earth
        earth.translate = glm::translate(I, glm::vec3(0,0,11));
        earth.rotate = glm::rotate(I, earth.rotation, glm::vec3(0,1,0)) * glm::rotate(I, 0.40907504363002f, glm::vec3(1,0,0));
        glm::mat4 earthaxial = glm::rotate(I, 0.40907504363002f, glm::vec3(1,0,0));
        model = sun.transform * glm::rotate(I, earthrotation, glm::vec3(0,1,0)) * earth.translate * earth.rotate;
        earth.transform = model;

        if(!keyboard_status.paused)
            earth.rotation += earthrotation_b;
        if(!keyboard_status.paused)
            earthrotation += earth_rotation_b;

        glUseProgram(shader.program);
        diffuse = glGetUniformLocation(shader.program, "diffuse");
        glUniform1ui(diffuse, 1);
        cloud = glGetUniformLocation(shader.program, "clouds");
        glUniform1ui(cloud, 0);
        glUniformMatrix4fv(modelUniform, 1, false, value_ptr(model));
        RenderScene(&earth.geometry, &shader, &earth.texture, false);

        // Earth Clouds
        clouds.translate = glm::translate(I, glm::vec3(0,0,11));
        clouds.rotate = glm::rotate(I, clouds.rotation, glm::vec3(0,1,0)) * rotate(I, 0.40907504363002f, glm::vec3(1,0,0));
        model = sun.transform * glm::rotate(I, cloudsrotation, glm::vec3(0,1,0)) * clouds.translate * clouds.rotate;
        clouds.transform = model;

        if(!keyboard_status.paused)
            clouds.rotation += cloudsrotation_b;
        if(!keyboard_status.paused)
            cloudsrotation += clouds_rotation_b;

        glUseProgram(shader.program);
        cloud = glGetUniformLocation(shader.program, "clouds");
        glUniform1ui(cloud, 1);
        diffuse = glGetUniformLocation(shader.program, "diffuse");
        glUniform1ui(diffuse, 1);
        glUniformMatrix4fv(modelUniform, 1, false, value_ptr(model));

        RenderScene(&clouds.geometry, &shader, &clouds.texture, true);

        // Moon
        moon.translate = glm::translate(I, glm::vec3(0,0,3));
        moon.rotate = glm::rotate(I, moon.rotation, glm::vec3(0,1,0));

        model = sun.transform * glm::rotate(I, earthrotation, glm::vec3(0,1,0)) * earth.translate * earthaxial * moon.rotate * moon.translate;
        if(!keyboard_status.paused)
            moon.rotation += moonrotation_b;

        glUseProgram(shader.program);
        diffuse = glGetUniformLocation(shader.program, "diffuse");
        glUniform1ui(diffuse, 1);
        cloud = glGetUniformLocation(shader.program, "clouds");
        glUniform1ui(cloud, 0);
        glUniformMatrix4fv(modelUniform, 1, false, value_ptr(model));
        RenderScene(&moon.geometry, &shader, &moon.texture, false);

        // Space
        model = glm::translate(I, glm::vec3(0,0,0));  
        glUseProgram(shader.program);
        diffuse = glGetUniformLocation(shader.program, "diffuse");
        glUniform1ui(diffuse, 0);
        cloud = glGetUniformLocation(shader.program, "clouds");
        glUniform1ui(cloud, 0);
        glUniformMatrix4fv(modelUniform, 1, false, value_ptr(model));
        RenderScene(&space.geometry, &shader, &space.texture, false);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // clean up allocated resources before exit
    DestroyGeometry(&sun.geometry);
    DestroyGeometry(&earth.geometry);
    DestroyGeometry(&moon.geometry);
    DestroyGeometry(&space.geometry);
    DestroyShaders(&shader);
    glfwDestroyWindow(window);
    glfwTerminate();

    std::cout << "Goodbye!" << std::endl;
    return 0;
}

// ==========================================================================
// SUPPORT FUNCTION DEFINITIONS

// --------------------------------------------------------------------------
// OpenGL utility functions

void QueryGLVersion()
{
    // query opengl version and renderer information
    std::string version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    std::string glslver = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
    std::string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    std::cout << "OpenGL [ " << version << " ] "
        << "with GLSL [ " << glslver << " ] "
        << "on renderer [ " << renderer << " ]" << std::endl;
}

bool CheckGLErrors()
{
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

// --------------------------------------------------------------------------
// OpenGL shader support functions

// reads a text file with the given name into a std::string
std::string LoadSource(const std::string &filename)
{
    std::string source;

    std::ifstream input(filename.c_str());
    if (input) {
        copy(std::istreambuf_iterator<char>(input),
            std::istreambuf_iterator<char>(),
            back_inserter(source));
        input.close();
    }
    else {
        std::cout << "ERROR: Could not load shader source from file "
            << filename << std::endl;
    }

    return source;
}

// creates and returns a shader object compiled from the given source
GLuint CompileShader(GLenum shaderType, const std::string &source)
{
    // allocate shader object name
    GLuint shaderObject = glCreateShader(shaderType);

    // try compiling the source as a shader of the given type
    const GLchar *source_ptr = source.c_str();
    glShaderSource(shaderObject, 1, &source_ptr, 0);
    glCompileShader(shaderObject);

    // retrieve compile status
    GLint status;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetShaderInfoLog(shaderObject, info.length(), &length, &info[0]);
        std::cout << "ERROR compiling shader:" << std::endl << std::endl;
        std::cout << source << std::endl;
        std::cout << info << std::endl;
    }

    return shaderObject;
}

// creates and returns a program object linked from vertex and fragment shaders
GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader)
{
    // allocate program object name
    GLuint programObject = glCreateProgram();

    // attach provided shader objects to this program
    if (vertexShader)   glAttachShader(programObject, vertexShader);
    if (fragmentShader) glAttachShader(programObject, fragmentShader);

    // try linking the program with given attachments
    glLinkProgram(programObject);

    // retrieve link status
    GLint status;
    glGetProgramiv(programObject, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glGetProgramInfoLog(programObject, info.length(), &length, &info[0]);
        std::cout << "ERROR linking shader program:" << std::endl;
        std::cout << info << std::endl;
    }

    return programObject;
}
