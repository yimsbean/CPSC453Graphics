// ==========================================================================
// Vertex program for barebones GLFW boilerplate
//
// Author:  Sonny Chan, University of Calgary
// Date:    December 2015
// ==========================================================================
#version 410

// location indices for these attributes correspond to those specified in the
// InitializeGeometry() function of the main program
layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec3 VertexColor;
layout(location = 2) in vec2 VertexUV;
// output to be interpolated between vertices and passed to the fragment stage
out vec3 colour;
out vec2 uv;
uniform vec2 movePixels;
uniform int questionNo;

//tweaking view frustrum acutally
vec4 trapezoid(vec4 pos){
    mat4 mat = mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 1, 0, 1
    );
    pos = pos * mat;
    return pos;
}

void main()
{
    if(questionNo == 4)
        gl_Position = trapezoid(vec4(VertexPosition.x - movePixels[0], VertexPosition.y  - movePixels[1], 0.0, 1.0));
    else
        gl_Position = vec4(VertexPosition.x - movePixels[0], VertexPosition.y, 0.0, 1.0);
        gl_PointSize = 10.0;

    colour = VertexColor;
    uv = VertexUV;
}
